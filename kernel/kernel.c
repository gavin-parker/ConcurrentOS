#include "kernel.h"

#define programs 10
pcb_t pcb[ programs ], *current = NULL;
int processCount = 0;
uint32_t stack = &tos_irq;
//maxAge is the number of fires of the timer before ageing the processes
int maxAge = 3;
int channels[programs][programs];
int queues[3][programs];
int rounds = 0;
int currentQueue = 0;
int q1Rounds = 3;
int q2Rounds = 2;
int q3Rounds = 1;
int thisRound = 0;
int debug = 1;
/*
SCHEDULER OVERVIEW:
Processes are assigned to 3 different process queues, each with different priorities.
The processes in each priority queue are run repeatedly, with higher priority queues having more iterations.
When a process has been descheduled it is 'aged' and eventually moved to a different priority queue.
'waiting' / yielding processes will naturally move quickly up the the lower priority queues.

IPC OVERVIEW:
IPC uses synchronous 1-way channels. Each process can send data to any other process
and can receive from multiple channels. When sending/getting data the process
waits for confirmation
*/



//Empties the process queues
void clearQueues(){
  for(int i=0;i<3;i++){
    for(int j=0;j<programs;j++){
      queues[i][j] = -1;
    }
  }
}

//adds a process to a queue
void addToQueue(int q, int pid){
  for(int i=0;i<programs;i++){
      if(queues[q][i] == -1){
        queues[q][i] = pid;
        return;
      }
  }
}
//remove all instances of a process from a queue
void removeFromQueue(int q, int pid){
  for(int i=0;i<programs;i++){
      if(queues[q][i] == pid){
        queues[q][i] = -1;
      }
  }
}

void incrementStack(){
    stack = stack + 0x00001000;
}

//finds the number of processes in a queue
int members(int q){
  int a = 0;
  for(int i=0;i<programs;i++){
    if(queues[q][i] > -1){
    a += 1;
  }
  }
  return a;
}

//move to next queue
void changeQueue(){
  int nextQueue = (currentQueue+1)%3;
  if(members(nextQueue) > 0){
    currentQueue = nextQueue;
  }else if(members((nextQueue+1)%3)>0){
    currentQueue = (nextQueue+1)%3;
  }else if(members(currentQueue)>0){
  }else{
    print("ALL QUEUES EMPTY. OH DEAR :( \n",0,0,0);
    while(1){

    }
  }
  //print("Moving to q%d \n",currentQueue,0,0);
}

//Get the pid of the next process in queue, or move to next queue if empty
int getNextProcess(int pid){
  if(members(currentQueue) == 0){
    changeQueue();
    rounds = 0;
    thisRound = 0;
  }
  //Queue has at least 1 member
  int j = 0;
  int first = -1;
  //find next position in queue and first process
  for(int i=0;i < programs;i++){
    if(queues[currentQueue][i] != -1 && first == -1){
      first = queues[currentQueue][i];
    }
    if(queues[currentQueue][i] == pid){
      j = (i+1)%programs;
      break;
    }
  }
  //Try and get next item in queue
  for(;j<programs;j++){
    if(queues[currentQueue][j] != -1){
      return queues[currentQueue][j];
    }
  }
  //otherwise choose from front of queue
  return first;

}


/*
  My custom scheduler assigns a priority to each process - lower being high priority.
  processes with priorty '-1' i.e maxINT are finished and can be overwritten
*/
void scheduler( ctx_t* ctx) {
  //print("entering scheduler\n",0,0,0);
  uint32_t id = current->pid;
  uint32_t nextId;
  nextId = getNextProcess(id);


  memcpy( &pcb[ id ].ctx, ctx, sizeof( ctx_t ) );
  memcpy( ctx, &pcb[ nextId ].ctx, sizeof( ctx_t ) );
  current = &pcb[ nextId ];

  pcb[id].priority++;
  if(pcb[id].priority >= maxAge){
    removeFromQueue(currentQueue,id);
    removeFromQueue((currentQueue+2)%3,id);
    removeFromQueue((currentQueue+1)%3,id);
    addToQueue((currentQueue+1)%3,id);
    pcb[id].priority = 0;
  }
  thisRound++;
  /*if(id != 0 && pcb[id].priority != -1){
    pcb[id].priority += 1;
  } */
  //print("P%d",nextId,0,0);

}

//kill a process
void killProcess(ctx_t* ctx ,int p){
  removeFromQueue(0,p);
  removeFromQueue(1,p);
  removeFromQueue(2,p);

  pcb[p].priority = -1;
  scheduler(ctx);

}

//gets the index of the next free slot in the pcb table
//if no free slots, increase total and return end index
int getSlot(){
  for(int i=0;i < processCount;i++){
    if(pcb[i].priority == -1){
      return i;
    }
  }
  processCount++;
  return (processCount-1);
}
//blank the channel contents for a new process
void wipeChannels(uint32_t pid){
  for(int i=0;i < programs;i++){
    channels[pid][i] = -1;
  }
}

//finds the next free slot in the pcb table and copies in the new process
void createProcess(uint32_t pc, uint32_t cpsr, uint32_t priority  ){
  pid_t pid = getSlot();
  memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
  pcb[pid].priority   = priority;
  pcb[ pid ].pid      = pid;
  pcb[ pid ].ctx.cpsr = cpsr;
  pcb[ pid ].ctx.pc   = pc;
  pcb[ pid ].ctx.sp   = stack + pid*0x00001000;
  addToQueue(priority,pid);
  print("created a process P%d\n",pid,0,0);
  wipeChannels(pid);
}

//FORK to create two seperate but identical processes
//copies stack space and ctx
int copyProcess(ctx_t * ctx){
  //get a new pid
  pid_t pid = getSlot();
  //blank out the new pcb because why not?
  memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
  //set the new pid
  pcb[ pid ].pid      = pid;
  //copy in the current context
  memcpy(&pcb[pid].ctx, ctx, sizeof(ctx_t));
  //allocate some new stack
  pcb[pid].priority = 0;
  pid_t currentPid = current->pid;
  //this bit is dodgy!
  uint32_t oldStack = stack + (currentPid-1)*0x00001000;
  uint32_t newStack = stack + (pid-1)*0x00001000;
  memset(newStack, 0, 0x00001000);
  memcpy(newStack, oldStack, 0x00001000);
  uint32_t sp = ctx->sp;
  uint32_t relativeSP = oldStack - sp; //dodgy!!
  pcb[pid].ctx.sp = newStack - relativeSP;
  pcb[pid].ctx.gpr[ 0 ] = 0;
  addToQueue(0,pid);
  wipeChannels(pid);
  //print("P%d",pid,0,0);
  return pid;
}



void kernel_handler_rst( ctx_t* ctx              ) {

  /* Configure the mechanism for interrupt handling by
   *
   * - configuring timer st. it raises a (periodic) interrupt for each
   *   timer tick,
   * - configuring GIC st. the selected interrupts are forwarded to the
   *   processor via the IRQ interrupt signal, then
   * - enabling IRQ interrupts.
   */
  incrementStack();
  TIMER0->Timer1Load     = 0x00100000; // select period = 2^20 ticks ~= 1 sec
  TIMER0->Timer1Ctrl     = 0x00000002; // select 32-bit   timer
  TIMER0->Timer1Ctrl    |= 0x00000040; // select periodic timer
  TIMER0->Timer1Ctrl    |= 0x00000020; // enable          timer interrupt
  TIMER0->Timer1Ctrl    |= 0x00000080; // enable          timer

  GICC0->PMR             = 0x000000F0; // unmask all            interrupts
  GICD0->ISENABLER[ 1 ] |= 0x00000010; // enable timer          interrupt
  GICC0->CTLR            = 0x00000001; // enable GIC interface
  GICD0->CTLR            = 0x00000001; // enable GIC distributor


  /* Initialise PCBs representing processes stemming from execution of
   * the two user programs.  Note in each case that
   *
   * - the CPSR value of 0x50 means the processor is switched into USR
   *   mode, with IRQ interrupts enabled, and
   * - the PC and SP values matche the entry point and top of stack.
   */
   clearQueues();
  createProcess(( uint32_t )( entry_terminal ), 0x50, 0);
  //createProcess(( uint32_t )( entry_P1 ), 0x50, 0);
  //createProcess(( uint32_t )( entry_P2 ), 0x50, 0);


  current = &pcb[ 0 ]; memcpy( ctx, &current->ctx, sizeof( ctx_t ) );
  irq_enable();
  return;
}


int do_fork(ctx_t* ctx){
  return copyProcess(ctx);
}

//kill the current process
int do_exit(ctx_t* ctx){
  pid_t pid = current->pid;
  memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
  pcb[pid].priority = -1;
  wipeChannels(pid);
  scheduler(ctx);
  return 0;
}
void stop(){

}

void kernel_handler_svc( ctx_t* ctx, uint32_t id ) {
  /* Based on the identified encoded as an immediate operandCount in the
   * instruction,
   *
   * - read  the arguments from preserved usr mode registers,
   * - perform whatever is appropriate for this system call,
   * - write any return value back to preserved usr mode registers.
   */
  switch( id ) {
    case 0x00 : { // yield()
      scheduler( ctx);
      break;
    }
    case 0x01 : { // write( fd, x, n )
      int   fd = ( int   )( ctx->gpr[ 0 ] );
      char*  x = ( char* )( ctx->gpr[ 1 ] );
      int    n = ( int   )( ctx->gpr[ 2 ] );

      for( int i = 0; i < n; i++ ) {
        PL011_putc( UART0, *x++ );
      }

      ctx->gpr[ 0 ] = n;
      break;
    }
    case 0x02 : { //fork()
      int r = do_fork(ctx);
      ctx->gpr[ 0 ] = r;
      break;
    }
    case 0x03 : {//close() - could use same handler as kill with diff. flag?
      do_exit(ctx);
      break;
    }
    case 0x04 : { // read( fd, x, n )
      int   fd = ( int   )( ctx->gpr[ 0 ] );
      char*  x = ( char* )( ctx->gpr[ 1 ] );
      int    n = ( int   )( ctx->gpr[ 2 ] );

      for( int i = 0; i < n; i++ ) {
        x[i] = PL011_getc( UART0 );
      }
      ctx->gpr[ 0 ] = n;
      break;
    }
    case 0x05 : {//kill(id)
      killProcess(ctx, ctx->gpr[0]);
      break;
    }
    //SEND CHANNEL
    case 0x06 : {
      int *pida = ctx->gpr[0];
      int pid = *pida;
      //print("pid:%d\n",pid,0,0);
      int *dat = pida[1];
      //print(" p%d sharing %d with %d \n",current->pid,dat,pid);
      int thisPid = current->pid;
      channels[pid][thisPid] = dat;
      break;
    }
    // GET CHANNEL
    case 0x07 : {
      int pid = current->pid;
      int chan = ctx->gpr[0];
      int dat = -1;
      int sender = -1;
      if(chan == -1){
        for(int i=0;i<programs;i++){
          if(channels[pid][i] != -1){
            dat = channels[pid][i];
            channels[i][pid] = 1;
            channels[pid][i] = -1;
            break;
          }
        }
    }else{
      dat = channels[pid][chan];
      channels[pid][chan] = -1;
    }
    ctx->gpr[0] = dat;
      break;
    }
    case 0x08 : {//getId() - returns the current pid, useful for IPC
      int pid = current->pid;
      ctx->gpr[0] = pid;
      break;
    }
    default   : { // unknown
        PL011_putc( UART0, 'O' ); TIMER0->Timer1IntClr = 0x01;
        PL011_putc( UART0, 'O' ); TIMER0->Timer1IntClr = 0x01;
        PL011_putc( UART0, 'P' ); TIMER0->Timer1IntClr = 0x01;
        PL011_putc( UART0, 'S' ); TIMER0->Timer1IntClr = 0x01;
      break;
    }
  }

  return;
}

void kernel_handler_irq(ctx_t* ctx) {
  irq_unable();
  // Step 2: read  the interrupt identifier so we know the source.
  int id = GICC0->IAR;
  // Step 4: handle the interrupt, then clear (or reset) the source.


  if( id == GIC_SOURCE_TIMER0 ) {
    //if the whole queue has had a timeslice
    //determine whether to go to next queue or repeat
    if(thisRound >= members(currentQueue)){
    thisRound = 0;
    rounds++;
    if(currentQueue == 0 && rounds >= q1Rounds){
      changeQueue();
      rounds = 0;
    }else if(currentQueue == 1 && rounds >= q2Rounds){
      changeQueue();
      rounds = 0;
    }else if(currentQueue == 2 && rounds >= q3Rounds){
      changeQueue();
      rounds = 0;
    }
  }
    scheduler(ctx);
    /*PL011_putc( UART0, 'T' );*/ TIMER0->Timer1IntClr = 0x01;
  }

  // Step 5: write the interrupt identifier to signal we're done.
  irq_enable();
  GICC0->EOIR = id;

  return;
}
