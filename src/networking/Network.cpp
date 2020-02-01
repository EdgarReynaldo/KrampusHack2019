


#include "Eagle.hpp"
#define CINTERFACE
#include "networking/Network.hpp"
#include "nilorea.h"
#include "networking/Server.hpp"



const CLIENTID BADCLIENT = (unsigned int)-1;



void* ReceiverThread(EagleThread* thread , void* data) {
   Network* network = (Network*)data;
   EAGLE_ASSERT(network && network->Ready());
   
   NETWORK* net = network->GetNetwork();
   if (!net) {
      return (void*)0;
   }

   while (!thread->ShouldStop()) {
      /// Wait for messages from our connections
      N_STR* netstr = netw_get_msg(net);
      
      if (netstr) {
         EagleEvent e;
         e.type = EAGLE_EVENT_NETWORK_RECV_MSG;
         e.source = dynamic_cast<EagleEventSource*>(network);
         e.window = 0;
         e.timestamp = (double)ProgramTime::Elapsed();
         e.network = new NETWORK_EVENT_DATA(network , net->link.ip?net->link.ip:"" , net->link.port?net->link.port:"" , (unsigned char*)netstr->data , netstr->length);
         network->EmitEvent(e , thread);
         free_nstr(&netstr);
      }
      else {
         int STATE = 0;
         int ENGINE_STATE = 0;
         netw_get_state(net , &STATE , &ENGINE_STATE);
//         EagleInfo() << StringPrintF("ReceiverThread %p says network state is %d , %d" , thread , STATE , ENGINE_STATE) << std::endl;
         if (STATE == NETW_EXIT_ASKED) {
            EagleWarn() << "Network asked us to exit" << std::endl;
            break;
         }
         if (ENGINE_STATE == NETW_THR_ENGINE_STOPPED) {
            Server* server = dynamic_cast<Server*>(network);
            if (!server) {
               EagleWarn() << "Network thread engine stopped on a client thread" << std::endl;
               break;
            }
         }
         sleep(1);
      }
   }
   
   EagleInfo() << "Network thread is exiting" << std::endl;
   
   return (void*)thread;
}



void Network::ShutdownThread(EagleThread** thread) {
   if (!thread) {return;}
   if (*thread) {
      EagleThread* t = *thread;
      if (t->Running()) {
         t->SignalToStop();
      }
      t->Join();
      EAGLE_ASSERT(sys);
      sys->FreeThread(t);
      *thread = 0;
   }
}



Network::Network(EagleSystem* esys , std::string classname , std::string objname) :
      EagleEventSource(),
      EagleObject(classname , objname , true),
      net(0),
      sys(esys),
      recv_thread(0)
{
   EAGLE_ASSERT(esys && esys->SystemUp());
   if (!esys || (esys && !esys->SystemUp())) {
      throw EagleException("Can't create Network without valid system pointer and system up!");
   }
}



void Network::Close() {
   ShutdownThread(&recv_thread);
   if (net) {
      netw_stop_thr_engine(net);
      netw_close(&net);
      net = 0;
   }
}



std::string Network::GetOurIP() {return (net?net->link.ip:"");}



std::string Network::GetOurPORT() {return (net?net->link.port:"");}



bool Network::SendPacket(const BinStream& bin) {
   return SendPacket(bin.Data() , bin.Size());
}



bool Network::SendPacket(const void* data , unsigned int SZ) {
   N_STR* nstr = 0;
   char_to_nstr_ex((char*)data , SZ , &nstr);
   return netw_add_msg(net , nstr);/// Not a memory leak, the network disposes of the nstr
}


