


#include "Eagle.hpp"
#define CINTERFACE
#include "Network.hpp"
#include "Nilorea.h"


void* NetworkThread(EagleThread* thread , void* data) {
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
         e.network = new NETWORK_EVENT_DATA(network , net->link.ip , net->link.port , (unsigned char*)netstr->data , netstr->length);
         network->EmitEvent(e , thread);
         free_nstr(&netstr);
      }
      else {
         int STATE = 0;
         int ENGINE_STATE = 0;
         netw_get_state(net , &STATE , &ENGINE_STATE);
         if (STATE == NETW_EXIT_ASKED || ENGINE_STATE == NETW_THR_ENGINE_STOPPED) {
            EagleWarn() << "Network asked us to exit or the thread engine stopped." << std::endl;
            break;
         }
         Sleep(1);
      }
   }
   
   return (void*)thread;
}



Network::Network(EagleSystem* esys) :
      net(0),
      sys(esys),
      thread(0),
      ourIP(""),
      ourPORT("")
{
   EAGLE_ASSERT(esys && esys->SystemUp());
   if (!esys || (esys && !esys->SystemUp())) {
      throw EagleException("Can't create Network without valid system pointer and system up!");
   }
}



bool Network::SendPacket(const BinStream& bin) {
   return SendPacket(bin.Data() , bin.Size());
}



bool Network::SendPacket(const void* data , unsigned int SZ) {
   N_STR* nstr = 0;
   char_to_nstr_ex((char*)data , SZ , &nstr);
   return netw_add_msg(net , nstr);/// Not a memory leak, the network disposes of the nstr
}


