#include "opendefs.h"
#include "openqueue.h"
#include "openserial.h"
#include "packetfunctions.h"
#include "IEEE802154E.h"
#include "IEEE802154_security.h"

//=========================== defination =====================================

#define HIGH_PRIORITY_QUEUE_ENTRY 5

//=========================== variables =======================================

openqueue_vars_t openqueue_vars;

//=========================== prototypes ======================================
void openqueue_reset_entry(OpenQueueEntry_t* entry);

//=========================== public ==========================================

//======= admin

/**
\brief Initialize this module.
*/
void openqueue_init() {
   uint8_t i;
   for (i=0;i<QUEUELENGTH;i++){
      openqueue_reset_entry(&(openqueue_vars.queue[i]));
   }
}

/**
\brief Trigger this module to print status information, over serial.

debugPrint_* functions are used by the openserial module to continuously print
status information about several modules in the OpenWSN stack.

\returns TRUE if this function printed something, FALSE otherwise.
*/
bool debugPrint_queue() {
   debugOpenQueueEntry_t output[QUEUELENGTH];
   uint8_t i;
   for (i=0;i<QUEUELENGTH;i++) {
      output[i].creator = openqueue_vars.queue[i].creator;
      output[i].owner   = openqueue_vars.queue[i].owner;
   }
   openserial_printStatus(STATUS_QUEUE,(uint8_t*)&output,QUEUELENGTH*sizeof(debugOpenQueueEntry_t));
   return TRUE;
}

//======= called by any component

/**
\brief Request a new (free) packet buffer.

Component throughout the protocol stack can call this function is they want to
get a new packet buffer to start creating a new packet.

\note Once a packet has been allocated, it is up to the creator of the packet
      to free it using the openqueue_freePacketBuffer() function.

\returns A pointer to the queue entry when it could be allocated, or NULL when
         it could not be allocated (buffer full or not synchronized).
*/
OpenQueueEntry_t* openqueue_getFreePacketBuffer(uint8_t creator) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   
   // refuse to allocate if we're not in sync
   if (ieee154e_isSynch()==FALSE && creator > COMPONENT_IEEE802154E){
     ENABLE_INTERRUPTS();
     return NULL;
   }
   
   // if you get here, I will try to allocate a buffer for you
   
   // if there is no space left for high priority queue, don't reserve
   if (openqueue_isHighPriorityEntryEnough()==FALSE && creator>COMPONENT_SIXTOP_RES){
      ENABLE_INTERRUPTS();
      return NULL;
   }
   
   // walk through queue and find free entry
   for (i=0;i<QUEUELENGTH;i++) {
      openqueue_sortpriority();
      if (openqueue_vars.queue[i].owner==COMPONENT_NULL) {
         openqueue_vars.queue[i].creator=creator;
         openqueue_vars.queue[i].owner=COMPONENT_OPENQUEUE;
         openqueue_vars.queue[i].priority=7;
         //OpenQueueEntry_t* pkt; //deneme
         //pkt=&openqueue_vars.queue[i]; //deneme
         //openqueue_addasn(&(openqueue_vars.queue[i])); //-ayca denemee
             
         ENABLE_INTERRUPTS(); 
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

OpenQueueEntry_t* openqueue_getFreePacketBuffer_withpriority(uint8_t creator, uint8_t priority ) {
    uint8_t i;
    uint8_t l;
    OpenQueueEntry_t* toDelete;
    INTERRUPT_DECLARATION();
    DISABLE_INTERRUPTS();
    
    // refuse to allocate if we're not in sync
    if (ieee154e_isSynch()==FALSE && creator > COMPONENT_IEEE802154E){
        ENABLE_INTERRUPTS();
        return NULL;
    }
    
    // if you get here, I will try to allocate a buffer for you

    //openserial_printf("6",1,'A'); // ayca - debug

    // if there is no space left for high priority queue, don't reserve
    if (openqueue_isHighPriorityEntryEnough()==FALSE && creator>COMPONENT_SIXTOP_RES){
        ENABLE_INTERRUPTS();
        return NULL;
    }

    uint8_t m=0;
    for (i=0;i<QUEUELENGTH;i++){ 
      if (openqueue_vars.queue[i].owner==COMPONENT_NULL){
         toDelete = &openqueue_vars.queue[i];               
         openqueue_reset_entry(toDelete);;  
      }

      if (openqueue_vars.queue[i].owner!=COMPONENT_NULL){
          m++ ;  
      }
  } 


  if (openqueue_vars.queue[0].owner==COMPONENT_NULL && openqueue_vars.queue[0].owner!=COMPONENT_NULL){
          //OpenQueueEntry_t temp;  // not sure about this part
          //temp=openqueue_vars.queue[i];
          for (i=0;i<m;i++) {
            openqueue_vars.queue[i]=openqueue_vars.queue[i+1];
          }
          //openqueue_vars.queue[9]=temp; // till here
          toDelete = &openqueue_vars.queue[m];               
          openqueue_reset_entry(toDelete);
    }
    
/*    for (i=0;i<QUEUELENGTH;i++) {
            openserial_printf(&openqueue_vars.queue[i].l2_asn, 5,'Z' );  
    }*/


    // walk through queue and find free entry
    for (i=0;i<QUEUELENGTH;i++) {
        openqueue_sortpriority();
        if (openqueue_vars.queue[i].owner==COMPONENT_NULL) {
            openqueue_vars.queue[i].creator=creator;
            openqueue_vars.queue[i].owner=COMPONENT_OPENQUEUE;
            openqueue_vars.queue[i].priority=priority;
            //OpenQueueEntry_t* pkt; //deneme
            //pkt=&openqueue_vars.queue[i]; //deneme
            //openqueue_addasn((&(openqueue_vars.queue[i]))); //-ayca denemee
            
            ENABLE_INTERRUPTS();
            return &openqueue_vars.queue[i];
        }       
    }
/*    if (priority==1){          
        for (l=0;l<QUEUELENGTH;l++) {
            if (openqueue_vars.queue[l].priority==8) {
                toDelete = &openqueue_vars.queue[l];               
                openqueue_reset_entry(toDelete);              
                openqueue_vars.queue[l].creator=creator;
                openqueue_vars.queue[l].owner=COMPONENT_OPENQUEUE;
                openqueue_vars.queue[l].priority=priority;
                openqueue_sortpriority();
                ENABLE_INTERRUPTS();
                return &openqueue_vars.queue[l];
              }
              else if (openqueue_vars.queue[l].priority==4) {
                toDelete = &openqueue_vars.queue[l];               
                openqueue_reset_entry(toDelete);              
                openqueue_vars.queue[l].creator=creator;
                openqueue_vars.queue[l].owner=COMPONENT_OPENQUEUE;
                openqueue_vars.queue[l].priority=priority;
                openqueue_sortpriority();
                ENABLE_INTERRUPTS();
                return &openqueue_vars.queue[l];
              }
          }
        }*/

    ENABLE_INTERRUPTS();
    return NULL;
}

void openqueue_sortpriority(){
  uint8_t b, j, k=0, l, c, a; 
    OpenQueueEntry_t temp;
    OpenQueueEntry_t* toDelete;
    INTERRUPT_DECLARATION();
    DISABLE_INTERRUPTS();

    uint8_t i;
    for ( i=0;i<QUEUELENGTH;i++)
    {
        if ( openqueue_vars.queue[i].l2_asn.bytes0and1==0)
        {
            //toDelete = &openqueue_vars.queue[i];               
            //openqueue_reset_entry(toDelete);
          openqueue_vars.queue[i].priority=9;
        }
        if (openqueue_vars.queue[i].l2_asn.byte4>>0 || openqueue_vars.queue[i].l2_asn.bytes2and3>>0 ){
            toDelete = &openqueue_vars.queue[i];               
            openqueue_reset_entry(toDelete);        
        }
    }



//=====================================heap sort/ascending order===========================================
/*     for (b = 1; b < QUEUELENGTH; b++){
        c = b;
        do{
            a = (c - 1) / 2;
            if (openqueue_vars.queue[a].priority < openqueue_vars.queue[c].priority){
                temp = openqueue_vars.queue[a];
                openqueue_vars.queue[a] = openqueue_vars.queue[c];
                openqueue_vars.queue[c] = temp;
            }
            c = a;
        } while (c != 0);
    }
    //openserial_printf("4",1,'A');// ayca- debug
    
    for (k = 0; k < QUEUELENGTH; k++)
    {
        j= QUEUELENGTH-1-k;
        temp = openqueue_vars.queue[0];
        openqueue_vars.queue[0] = openqueue_vars.queue[j];    // swap max element with rightmost leaf element 
        openqueue_vars.queue[j] = temp;
        a = 0;
        do
        {
            c = 2 * a + 1;    // left node of root element 
            if ((openqueue_vars.queue[c].priority < openqueue_vars.queue[c + 1].priority) && c < j-1)
                c++;
            if (openqueue_vars.queue[a].priority<openqueue_vars.queue[c].priority && c<j)    // again rearrange to max heap array 
            {
                temp = openqueue_vars.queue[a];
                openqueue_vars.queue[a] = openqueue_vars.queue[c];
                openqueue_vars.queue[c] = temp;
            }
            a = c;
        } while (c < j);
    }*/

//====================================heap sort until here========================================================  

    for (i=0;i<QUEUELENGTH;i++) {
      if(openqueue_vars.queue[i].owner!=COMPONENT_NULL){
        k++;
      }
    }  

/*       for (b = 1; b < k; b++){  //insertion sort --> sensor crashes/no response/must reset manuelly
          k = openqueue_vars.queue[b].priority;
          temp= openqueue_vars.queue[b];
          c = b-1;
          while (c > -1 && openqueue_vars.queue[c].priority < k){
              openqueue_vars.queue[c+1] = openqueue_vars.queue[c];
              c = c-1;
          }
          openqueue_vars.queue[c+1] = temp;
        }*/

      for (c = 1 ; c <= QUEUELENGTH - 1; c++) {
        b = c;
 
        while ( b > 0 && openqueue_vars.queue[b-1].priority > openqueue_vars.queue[b].priority) {
          temp          = openqueue_vars.queue[b];
          openqueue_vars.queue[b]   = openqueue_vars.queue[b-1];
          openqueue_vars.queue[b-1] = temp;
          b--;
        }
      }

}

owerror_t openqueue_addasn(OpenQueueEntry_t* pkt){ //-- ayca denemeee
          asn_t currentasn = ieee154e_getASN();
          //openserial_printf("11111", 5,'A' );
          memcpy(&pkt->l2_asn, &currentasn, sizeof(asn_t));
}

void print_ASNofQueue(){
  uint8_t i;
  uint8_t m=0;
  for (i=0;i<QUEUELENGTH;i++){ 
      if (openqueue_vars.queue[i].owner!=COMPONENT_NULL){
          m++ ;  
      }
  } 
  // m+1 because if a packet leaves buffer and a new one does not enter, it prints out the the number of
  // openqueue_vars.queue[i].owner!=COMPONENT_NULL and does not print last member
  // the empty packet place has the ASN number the delivered packet had 
    for (i=0;i<m+1;i++) {
            openserial_printf(&openqueue_vars.queue[i].l2_asn, 5,'Z' );  
    }
}

void print_PriorityofQueue(){
      int i = 0, len = 0, count = 10;
      char string[64] = { 0 };   

      for (; i < count; ++i)
      {
          len += sprintf(&string[len], "%d%s", openqueue_vars.queue[i].priority, i == count - 1 ? "\n" : ", ");
      }

      openserial_printf(string,30,'B');
}


/**
\brief Free a previously-allocated packet buffer.

\param pkt A pointer to the previsouly-allocated packet buffer.

\returns E_SUCCESS when the freeing was succeful.
\returns E_FAIL when the module could not find the specified packet buffer.
*/
owerror_t openqueue_freePacketBuffer(OpenQueueEntry_t* pkt) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (&openqueue_vars.queue[i]==pkt) {
         if (openqueue_vars.queue[i].owner==COMPONENT_NULL) {
            // log the error
            openserial_printCritical(COMPONENT_OPENQUEUE,ERR_FREEING_UNUSED,
                                  (errorparameter_t)0,
                                  (errorparameter_t)0);
         }
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
         ENABLE_INTERRUPTS();
         return E_SUCCESS;
      }
   }
   // log the error
   openserial_printCritical(COMPONENT_OPENQUEUE,ERR_FREEING_ERROR,
                         (errorparameter_t)0,
                         (errorparameter_t)0);
   ENABLE_INTERRUPTS();
   return E_FAIL;
}

/**
\brief Free all the packet buffers created by a specific module.

\param creator The identifier of the component, taken in COMPONENT_*.
*/
void openqueue_removeAllCreatedBy(uint8_t creator) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++){
      if (openqueue_vars.queue[i].creator==creator) {
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
      }
   }
   ENABLE_INTERRUPTS();
}

/**
\brief Free all the packet buffers owned by a specific module.

\param owner The identifier of the component, taken in COMPONENT_*.
*/
void openqueue_removeAllOwnedBy(uint8_t owner) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++){
      if (openqueue_vars.queue[i].owner==owner) {
         openqueue_reset_entry(&(openqueue_vars.queue[i]));
      }
   }
   ENABLE_INTERRUPTS();
}

//======= called by RES

OpenQueueEntry_t* openqueue_sixtopGetSentPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_IEEE802154E_TO_SIXTOP &&
          openqueue_vars.queue[i].creator!=COMPONENT_IEEE802154E) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

OpenQueueEntry_t* openqueue_sixtopGetReceivedPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_IEEE802154E_TO_SIXTOP &&
          openqueue_vars.queue[i].creator==COMPONENT_IEEE802154E) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

//======= called by IEEE80215E

OpenQueueEntry_t* openqueue_macGetDataPacket(open_addr_t* toNeighbor) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();

    // first to look the sixtop RES packet
    for (i=0;i<QUEUELENGTH;i++) {
       if (
           openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
           openqueue_vars.queue[i].creator==COMPONENT_SIXTOP_RES &&
           (
               (
                   toNeighbor->type==ADDR_64B &&
                   packetfunctions_sameAddress(toNeighbor,&openqueue_vars.queue[i].l2_nextORpreviousHop)
               ) || toNeighbor->type==ADDR_ANYCAST
           )
       ){
          ENABLE_INTERRUPTS();
          return &openqueue_vars.queue[i];
       }
    }
  
   if (toNeighbor->type==ADDR_64B) {
      // a neighbor is specified, look for a packet unicast to that neigbhbor
      for (i=0;i<QUEUELENGTH;i++) {
         if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
            packetfunctions_sameAddress(toNeighbor,&openqueue_vars.queue[i].l2_nextORpreviousHop)
          ) {
            ENABLE_INTERRUPTS();
            return &openqueue_vars.queue[i];
         }
      }
   } else if (toNeighbor->type==ADDR_ANYCAST) {
      // anycast case: look for a packet which is either not created by RES
      // or an KA (created by RES, but not broadcast)
      for (i=0;i<QUEUELENGTH;i++) {
         if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
             ( openqueue_vars.queue[i].creator!=COMPONENT_SIXTOP ||
                (
                   openqueue_vars.queue[i].creator==COMPONENT_SIXTOP &&
                   packetfunctions_isBroadcastMulticast(&(openqueue_vars.queue[i].l2_nextORpreviousHop))==FALSE
                )
             )
            ) {
            ENABLE_INTERRUPTS();
            return &openqueue_vars.queue[i];
         }
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

bool openqueue_isHighPriorityEntryEnough(){
    uint8_t i;
    uint8_t numberOfEntry;
    
    numberOfEntry = 0;
    for (i=0;i<QUEUELENGTH;i++) {
        if(openqueue_vars.queue[i].creator>COMPONENT_SIXTOP_RES){
            numberOfEntry++;
        }
    }
    
    if (numberOfEntry>QUEUELENGTH-HIGH_PRIORITY_QUEUE_ENTRY){
        return FALSE;
    } else {
        return TRUE;
    }
}

OpenQueueEntry_t* openqueue_macGetEBPacket() {
   uint8_t i;
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   for (i=0;i<QUEUELENGTH;i++) {
      if (openqueue_vars.queue[i].owner==COMPONENT_SIXTOP_TO_IEEE802154E &&
          openqueue_vars.queue[i].creator==COMPONENT_SIXTOP              &&
          packetfunctions_isBroadcastMulticast(&(openqueue_vars.queue[i].l2_nextORpreviousHop))) {
         ENABLE_INTERRUPTS();
         return &openqueue_vars.queue[i];
      }
   }
   ENABLE_INTERRUPTS();
   return NULL;
}

//=========================== private =========================================

void openqueue_reset_entry(OpenQueueEntry_t* entry) {
   //admin
   entry->creator                      = COMPONENT_NULL;
   entry->owner                        = COMPONENT_NULL;
   entry->payload                      = &(entry->packet[127 - IEEE802154_SECURITY_TAG_LEN]); // Footer is longer if security is used
   entry->length                       = 0;
   entry->priority                     = 9;
   //l4
   entry->l4_protocol                  = IANA_UNDEFINED;
   entry->l4_protocol_compressed       = FALSE;
   //l3
   entry->l3_destinationAdd.type       = ADDR_NONE;
   entry->l3_sourceAdd.type            = ADDR_NONE;
   //l2
   entry->l2_nextORpreviousHop.type    = ADDR_NONE;
   entry->l2_frameType                 = IEEE154_TYPE_UNDEFINED;
   entry->l2_retriesLeft               = 0;
   entry->l2_IEListPresent             = 0;
   entry->l2_isNegativeACK             = 0;
   entry->l2_payloadIEpresent          = 0;
   //l2-security
   entry->l2_securityLevel             = 0;


  entry->l2_asn.byte4 = 0;
  entry->l2_asn.bytes2and3 = 0;
  entry->l2_asn.bytes0and1 = 0;
}

