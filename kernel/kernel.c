#include "kernel.h"

/* Since we *know* there will be 2 processes, stemming from the 2 user
 * programs, we can
 *
 * - allocate a fixed-size process table (of PCBs), and use a pointer
 *   to keep track of which entry is currently executing, and
 * - employ a fixed-case of round-robin scheduling: no more processes
 *   can be created, and neither is able to complete.
 */
#define programs 10
pcb_t pcb[ programs ], *current = NULL;
int processCount = 0;
uint32_t stack = &tos_irq;
//maxAge is the number of fires of the timer before ageing the processes
int agetime = 0;
int maxAge = 3;

int rand = 0;
int channels[programs][2];

void incrementStack(){
    stack = stack + 0x00001000;
}
void age(){
  for(int i=0;i < programs; i++){
    if(pcb[i].priority != -1 && pcb[i].priority > 0){
      pcb[i].priority -= 1;
    }
  }
}

//get the index of the proc with highest priority
//breaks ties at 'random'
// O(n) - very naughty!
int getNextProcess(){
  int id = 0;
  uint32_t p = -1;
  for(int i=0;i< processCount;i++){
    if(pcb[i].priority < p){
      p = pcb[i].priority;
      id = i;
    }else if((pcb[i].priority == p) && ((pcb[i].ctx.sp / 8) % 2 == (rand % 2)) ){
      p = pcb[i].priority;
      id = i;
    }
  }
  return id;
}

//increase the priority of a process with index i
void increasePriority( int i, int age){
  if(age > pcb[i].priority){
    pcb[i].priority = 0;
  }else{
  pcb[i].priority -= age;
}
  return;
}

int getNewProcess(int this){
  int id = 0;
  uint32_t p = -1;
  for(int i=0;i< processCount;i++){
    if(i != this){
    if(pcb[i].priority < p){
      p = pcb[i].priority;
      id = i;
    }else if((pcb[i].priority == p) && ((pcb[i].ctx.sp / 8) % 2 == (rand % 2)) ){
      p = pcb[i].priority;
      id = i;
    }
  }
  }
  return id;
}
/*
  My custom scheduler assigns a priority to each process - lower being high priority.
  processes with priorty '-1' i.e maxINT are finished and can be overwritten
*/
void scheduler( ctx_t* ctx , int immediate) {
  //print("entering scheduler\n",0,0,0);
  uint32_t id = current->pid;
  uint32_t nextId;
  if(immediate < 0){
   nextId = getNextProcess();
   nextId = (id+1)%processCount;
 }else{
   nextId = immediate;
 }
  memcpy( &pcb[ id ].ctx, ctx, sizeof( ctx_t ) );
  memcpy( ctx, &pcb[ nextId ].ctx, sizeof( ctx_t ) );
  current = &pcb[ nextId ];
  /*if(id != 0 && pcb[id].priority != -1){
    pcb[id].priority += 1;
  } */
  //print("P%d",nextId,0,0);

}
void killProcess(ctx_t* ctx ,int p){
  pcb[p].priority = -1;
  scheduler(ctx,-1);

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

//finds the next free slot in the pcb table and copies in the new process
void createProcess(uint32_t pc, uint32_t cpsr, uint32_t priority  ){
  pid_t pid = getSlot();
  memset( &pcb[ pid ], 0, sizeof( pcb_t ) );
  pcb[pid].priority   = priority;
  pcb[ pid ].pid      = pid;
  pcb[ pid ].ctx.cpsr = cpsr;
  pcb[ pid ].ctx.pc   = pc;
  pcb[ pid ].ctx.sp   = stack + pid*0x00001000;
  print("created a process P%d\n",pid,0,0);
  channels[pid][1] = -1;
}

//copy whole ctx to new process
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
  int difference = (pid - currentPid);
  memcpy(&stack + pid*0x00001000, &stack + currentPid*0x00001000, 0x00001000);
  //pcb[pid].ctx.sp += difference*0x00001000;
  pcb[pid].ctx.gpr[ 0 ] = 0;
  channels[pid][1] = -1;
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

void do_share(int pid, int dat){
  //print("[0]:%d, [1]:%d \n",dat,current->pid,0);
  channels[pid][0] = dat;
  channels[pid][1] = current->pid;
  //print("[0]:%d, [1]:%d \n",dat,channels[pid][1],0);

}

int do_exit(ctx_t* ctx){
  pid_t pid = current->pid;
  pcb[pid].priority = -1;
  scheduler(ctx,-1);
  return 0;
}

void kernel_handler_svc( ctx_t* ctx, uint32_t id ) {
  /* Based on the identified encoded as an immediate operand in the
   * instruction,
   *
   * - read  the arguments from preserved usr mode registers,
   * - perform whatever is appropriate for this system call,
   * - write any return value back to preserved usr mode registers.
   */
  switch( id ) {
    case 0x00 : { // yield()
      scheduler( ctx ,-1);
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
    case 0x03 : {
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
    case 0x05 : {
      killProcess(ctx, ctx->gpr[0]);
      break;
    }
    case 0x06 : {
      int *pida = ctx->gpr[0];
      int pid = *pida;
      //print("pid:%d\n",pid,0,0);
      int *dat = pida[1];
      //int dat = ctx->gpr[1];
      do_share(pid, dat);
      //scheduler(ctx,pid);
      break;
    }
    case 0x07 : {
      int pid = current->pid;

      if(channels[pid][1] == -1){
        ctx->gpr[0] = 0;
        ctx->gpr[1] = -1;
      }else{
        ctx->gpr[0] = channels[pid][0];
        ctx->gpr[1] = channels[pid][1];
        channels[pid][1] = -1;
      }
      break;
    }
    case 0x08 : {
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
    rand++;
    scheduler(ctx,-1);
    agetime++;
    if(agetime >= maxAge){
      age();
      agetime = 0;
    }
    /*PL011_putc( UART0, 'T' );*/ TIMER0->Timer1IntClr = 0x01;
  }

  // Step 5: write the interrupt identifier to signal we're done.
  irq_enable();
  GICC0->EOIR = id;

  return;
}
