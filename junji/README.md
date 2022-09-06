# 운영체제
- `컴퓨터 하드웨어 바로 위에 설치되는 소프트웨어 계층`으로서 모든 컴퓨터 시스템의 필수적인 부분이다. 본 강좌에서는 `운영체제의 개념과 역할`, `운영체제를 구성하는 각 요소` 및 `그 알고리즘의 핵심적인 부분`에 대해 기초부터 학습한다.

- 운영체제 개요
- 컴퓨터시스템의 구조
- 프로세스 관리
- CPU 스케줄링
- 병행 제어
- 데드락
- 메모리 관리
- 가상 메모리
- 파일 시스템
- 입출력 시스템
- 디스크 관리

> keyword
```
프로세스 생성
fork
exec
```
# [11강. 프로세스 관리 5]

### 프로세스 생성(Process Creation)
- 부모 프로세스가 자식 프로세스 생성(system call)
- 프로세스의 트리(계층구조)형성
- 프로세스는 자원을 필요로 함
	- 운영체제로부터 받음
	- 부모와 공유
- 자원을 공유
	- 부모와 자식이 모든 자원을 공유하는 모델
	- 일부를 공유하는 모델
	- 전혀 공유하지 않는 모델
- 수행(Execution)
	- 부모와 자식은 공존하며 수행되는 모델
	- 자식이 종료(terminate)될 떄까지 부모가 기다리는(wait)모델 

### fork() 시스템 콜
- 주소 공간(Adress space) // code data stack
	- 자식은 부모의 공간을 복사함(binary and OS data)
	- 자식은 그 공간에 새로운 프로그램을 올림
- 유닉스
	- fork() 시스템 콜이 새로운 프로세스를 생성
		- 부모를 그대로 복사(OS data except PID + binary)
		- 주소 공간 할당
	- fork 다음에 이어지는 exec () 시스템 콜을 통해 새로운 프로그램을 메모리에 올림
```c
pid = fork();
if (pid > 0) // parent
else // child
```

### 프로세스 종료(Process Termination)
- 프로세스가 마지막 명령을 수행한 후 운영체제에게 이를 알려줌(exit)
	- 자식이 부모에게 output data를 보냄(via wait)
	- 프로세스의 각종 자원들이 운영체제에게 반납됨
- 부모 프로세스가 자식의 수행을 종료시킴(abort)
	-  자식이 할당 자원의 한계치를 넘어섬
	- 자식에게 할당된 태스크가 더 이상 필요하지 않음
	- 부모가 종료(exit)하는 경우
		- 운영체제는 부모 프로세스가 종료하는 경우 자식이 더 이상 수행되도록 두지 않는다.
		- 단계적인 종료.
> keyword

### exec()시스템 콜
```c
int main(void)
{
	printf("hello\n");
	execlp("/bin/date", "/bin/date", (char *)0); // 3번쨰 아규먼트는 전달할 거. //hello가 아니라 date를 출력하는 ...
	printf("haha\n"); // 덮어 씌워졌기 때문에 실행되지 않는다.k
}
```
프로세스의 상태(ready, running, blocked(wait, sleep))
인터럽트 
swap in, out
Thread
	- 구성
	- 공유하는 부분
장점
```

# [10강. 프로세스 관리 4]
- 프로세스의 상태도(사진)
- swap in, out
	- swap out : 메모리에서 통째로 쫓겨남
	- swap in : 다시 메모리로 들어옴
- process1이 system call을 호출하고, CPU가 운영체제한테 넘어갔을 때 A를 무슨 상태라고 부르는가? -> A가 여전히 일을 하고 있다고 봄이 타당
- process1이 interrupt가 걸려도, a는 여전히 실행되고 있다(running).(간주한다), process2가 I/O 처리를 끝내고 들어왔지만, b가 running했다는 표현을 쓰지 않고 직전에 들어온 프로세스가 커널모드에서 동작한다. (로 간주)
- DISK I/O => Hardware 인터럽트기도 하고 software인터럽트도 하다. I/O가 시작될 때는 System call을 해서 요청. (software interrupt), I/O가 끝났을 떈 Disk controller가 I/O에게 끝났다고 알리기에 (hardware interrupt).
 
### Thread 
- A `thread`(or `lightweight` process) is a basic util of CPU utilization
- Thread의 구성
	- program counter
	- register set
	- stack space
- Thread가 동료 Thread와 공유하는 부분(=task)
	- code section
	- data section
	- OS resources
- 전통적인 개념의 heavyweight process는 하나의 thread를 가지고 있는 task로 볼 수 있다.

- web browser에서 스레드로 여러개 만들거나, 보안 위해 각각 프로세스로.
	- 여튼 구현하기 나름이다.

### 쓰레드의 장점
- 다른 스레드로 구성된 데스크 구조에서는 하나의 서버 스레드가 blocked(waiting)상태인 동안에도 동일한 데스크 내의 다른 스레드가 실행(running)되어 빠른 처리를 할 수 있다.
- 동일한 일을 수행하는 다중 스레드가 협력하여 높은 처리율(throughput)과 성능 향상을 만들 수 있다.
- 스레드를 사용하면 병렬성을 높일 수 있다.
ex) 하나의 스레드가 웹페이지에 이미지를 받아오는 동안에 화면에 표시할 수 있는 텍스트라도 먼저 표시를 해주면..

### Thread
- OS관리용 정보
	- Pointer, Process state, process number
- CPU관련 정보(Thread)
	- program counter, register
- 자원 관련 정보
	- memory limits
	- list of open files

### Benefits of threads
- Responsiveness
- Resource sharing
- economy 
	- createing & cpu switching thread (rather than a process)
	- ex)solaris, overhead 30배, 5배
- utilization of MP architecture
	- each thread may be running in parallel on a different processor

### Implementation of Thread
- Some are supported by kernel (운영체제가 스레드의 존재를 앎. 서로 다른 스레드가 마치 프로세스처럼 보여서 CPU스케쥴링을 할 때 A라는 스레드에서 B라는 스레드로 프로세스를 넘겨야지) => Kernel Threads
- Others are supported by library(운영체제가 스레드의 존재를 모름, 프로세스 내부에서 A라는 스레드를 실행하다가 A가 I/O를 하러가면 운영체제에게 비동기식 입출력을 요청해서 바로 CPU를 받고 B라는 스레드한테 스레드를 넘기고) => User Threads
- Some are real-time threads
 
# [9강. 프로세스 관리 3]
### 스케쥴러(scheduler)
- 1. Long-term scheduler(장기 스케쥴러 or job scheduler) //메모리를 주는 문제
	- 시작 프로세스 중 어떤 것들을 ready queue로 보낼지 결정
	- 프로세스에 memory(및 각종 자원)을 주는 문제
	- degree of Multiprogramming을 제어
	- times sharing system에는 보통 장기 스케쥴러가 없음(무조건 ready)
- 2. Short-term scheduler(단기 스케쥴러 or CPU scheduler)
	- 어떤 프로세스를 다음번에 running시킬 지 결정
	- 프로세스에 CPU를 주는 문제
	- 충분히 빨라야 함(millisecond 단위)
- 3. Medium-term scheduler(중기 스케쥴러 or Swapper) // time sharing시스템에서 장기 스케쥴러가 없기 때문에 중기 스케쥴러가 있다. 메모리 경합문제
	- 여유 공간 마련을 위해 프로세스 통째로 메모리에서 디스크로 쫓아냄
	- 프로세스에게서 memory를 뺏는 문제
	- degree of Multiprogramming을 제어
cf.
	메모리에 올라가있는 프로그램의 수 : degree of multiprogramming
	
### 프로세스의 상태(process state)
- Running, Ready, Blocked(wait, sleep)
- Suspended(stopped) // 중기 스케쥴러에 의한 상태, 이외에도 여러 원인 
	- 외부적인 이유로 프로세스의 수행이 정지된 상태
	- 프로세스는 통째로 디스크에 swap out된다.
	- 사용자가 프로그램을 일시정지 시킨 경우(break key)
		- 시스템이 여러 이유로 프로세스를 잠시 중단 시킴
		- 메모리에 너무 많은 프로세스가 올라와 있을 때
cf. 
	- Blocked: 자신이 요청한 event가 만족되면 Ready
	- Suspended: 외부에서 resume해 주어야 Active
// 실행이라는 것이 CPU사용하는 것만이 아니다. I/O를 기다리는 프로세스도 살아 있는 거임. Suspended는 아예 정지된 상태임.
> 2022.09.05(월)
```
- device controller에 펌웨어 -> 키보드 마우스에도 다 펌웨어가 있다.
```

> keyword
```c
프로세스
프로세스의 상태(running ready blocked)
PCB
Context Switch
```
# [8강. 프로세스 관리 2]
### 프로세스의 개념
- Process is `a program in execution`.
- 프로세스의 문맥(context)
	- cpu 수행 상태를 나타내는 하드웨어 문맥
		- program Counter
		- 각종 register
	- 프로세스의 주소 공간
		- code, data, stack
	- 프로세스 관련 커널 자료 구조
		- PCB(Process control block)
		- Kernel stack
 
### 프로세스의 상태
- 프로세스는 상태(state)가 변경되며 수행된다.
	- Running
		- CPU를 잡고 instruction을 수행중인 상태
	- Ready
		- CPU를 기다리는 상태(메모리 등 다른 조건을 모두 만족하고)
	- Blocked(wait, sleep)
		- CPU를 주어도 당장 instruction을 수행할 수 없는 상태
		- process 자신이 요청한 event(ex: I/O)가 즉시 만족되지 않아 이를 기다리는 상태
		- ex) 하드디스크에서 파일을 읽어와야 하는 경우
	- New: 프로세스가 생성중인 상태
	- Terminated: 수행이 끝난 상
```
여러 큐를 둬서 프로세스 상태가 어떻고 하는 걸 
운영체제가 Data영역에 있는 PCB를 통해서 관리한다. 
// 프로세스 마다 PCB를 두고 있다.

온전하게 프로세스가 되면 ready상태(cpu만 있으면 당장 실행)
cpu에게 당장 필요한 부분은 메모리에. ready상태는 프로세스가 메모리에 존재

CPU를 계속 쓰고 싶은데 못 쓰는 경우
1. timer interrupt
2. CPU기계어 실행, 오래걸리는 작업(I/O or event wall) -> blocked status
3. 본인의 일을 다해서 종료
```

### PCB (Process Control Block)
- 운영체제가 각 프로세스를 관리하기 위해 프로세스당 유지하는 정보
- 다음의 구성 요소를 가진다(구조체)
	- 1. OS가 관리상 사용하는 정보
		- Process state, Process ID
		- scheduling information, priority
	- 2. CPU 수행 관련 하드웨어 값
		- Program counter, registers
	- 3. 메모리 관련
		- Code, data, stack의 위치정보
	- 4. 파일 관련
		- Open file descriptors

- PCB는 어디에 있는 걸까? 운영체제(커널)가 가지고 있는 data영역에 각 프로세스별 정보가 있다.
- 왜 PCB에 program counter라던지 registers를 들고 다닐까?
	- 매번 CPU를 뺏길 때는 이 프로세스가 어디까지 수행되었는지 알아야 하기 때문이다.

### 문맥 교환(context switch)
- CPU를 한 프로세스에서 다른 프로세스로 넘겨주는 과정
- CPU가 다른 프로세스에게 넘어갈 때 운영체제는 다음을 수행
	- CPU를 내어주는 프로세스의 상태를 그 프로세스의 PCB에 저장
	- CPU를 새롭게 얻는 프로세스의 상태를 PCB에서 읽어옴.
- 기존 프로세스 cache memory를 싹다 비워야하는 비용이 되게 크다고 하더라(flush)
```
CPU가 하나의 프로세스에서 다른 프로세스로 넘어가는 과정을 의미.
문맥교환과 문맥교환 아닌게 있다.
- system call이나 interrupt 발생시 반드시 context switch가 일어나는 건 아니다.
	- program A cpu, interrupt or system call -> Kernel mode. 
	  (즉 사용자 CPU에서 운영체제 커널로 CPU가 넘어간 것을 문맥교환이라고 하진 않는다 A->Kernel->A) 지극히 일부만 save.
	- A, interrupt(다른 프로그램) -> 커널이 키보드 입력 처리 ->
		(이 과정도 CPU를 빼앗긴게 아니다.)

Context switch
1. timer
2. I/O 
```

### 프로세스를 스케쥴링하기 위한 큐
- job queue
	- 현재 시스템 내에 있는 모든 프로세스의 집합
- Ready queue
	- 현재 메모리 내에 있으면서 CPU를 잡아서 실행되기를 기다리는 프로세스의 집합
- Device queue
	- I/O device의 처리를 기다리는 프로세스의 집합
- 프로세스들은 각 큐들을 오가며 수행된다.
### 
> keyword
```
- process
- physical memory, virtual memory
- 커널 주소 공간의 내용(code, data, stack)
- kernel mode, user mode
```
# [7강. 프로세스 관리 1]
### 프로그램의 실행(메모리 load)
- 프로그램을 더블 클릭하면 메모리에 올라가 프로세스가 된다.
- 프로그램이 실행될 때 프로그램만의 독자적인 주소 공간(address space)이 만들어지고 이것을 `virtual memory` 가상 메모리라고 한다. 당장 필요한 부분은 `physical memory`에 올라가게 되고, 그렇지 않은 부분은 `swap area`에 있다. 
	- 나중에 메모리 관리할 때 설명이 추가로 되겠지만, virtual memory에서 `code` 영역 같은 부분은 `File System`에 실행 파일 형태로 존재하게 된다.
- virtual memory와 physical memory사이에 `주소 변환`이 필요하다.
- virtual memory는 <stack> <data> <code>영역이 있는데, `code`영역은 실제 `cpu에서 수행할 기계어들이 위치하는 부분`이다.

### 커널 주소 공간의 내용
1. code
	- 커널 코드
		- 시스템 콜, 인터럽트 처리 코드
		- 자원 관리를 위한 코드
		- 편리한 서비스 제공을 위한 코드
2. data
	- PCB PCB (모든 프로세스들을 관리하기 위한 자료구조를 가지고 있음)
	- CPU MEM DISK (모든 하드웨어들을 관리하기 위한 자료구조를 가지고 있음)
3. stack
	- Process A의 커널 스택
	- Process B의 커널 스택

### 사용자 프로그램이 사용하는 함수
- 사용자 정의 함수
	- 자신의 프로그램에서 정의한 함수
- 라이브러리 함수
	- 자신의 프로그램에서 정의하지 않고 갖다 쓴 함수
	- 자신의 프로그램의 실행 파일에 포함되어 있다.
- 커널 함수
	- 운영체제 프로그램의 함수
	- 커널 함수의 호출 = 시스템 콜
- 내 함수나 라이브러리 함수를 사용하는 것은 내 프로그램 안에서 program counter값만 바꾸어서 다른 위치에 있는 기계어를 실행하는 것이다. 단 system call은 가상 메모리 공간을 가로 질러서 영역이 완전 바뀌는 것이다. cpu제어권을 운영체제한테 넘어가게 한다.
- CPU 옆에는 mode bit 가 붙어있다. 

> 9.5(월)
```
- 컴퓨터 시스템 구조
	- register에 program counter, mode bit
- interrupt(program counter)
	 - interrupt service routine
	 - interrupt vector
- system call
- 운영체제 한테 cpu가 넘어가는 경우?
- I/O device controller, I/O buffer
- device driver, device controller
- DMA
```

> 9.26(화)
```
- synchornous I/O
- asynchronus I/O
- 저장장치 계층 구조(캐싱)
```

# [6강. 컴퓨터 시스템 구조] 
- 인터럽트가 들어오면 프로그램 카운터는 운영체제를 가리킨다. register에 program counter가 있는데 이는 다음 기계어를 실행할 위치를 가리키고. 운영체제 코드가 실행될 때 mode bit0. mode bit1(제한된 기계어만 실행)

운영체제한테 cpu가 넘어가는 경우는 어떤 경우가 있나요?
-> interrupt가 있을 경우에 넘어가고, timer에 의해서 넘어감. 해당 프로세스의 실행이 완료되었을 떄?
1. 하드웨어 장치들이 인터럽트를 걸었을 경우
2. 프로그램 소프트웨어가 직접 IRQ(Interrupt Request line)을 설정해서 운영체제한테 CPU가 넘어가는. (ex) system call) - 본연의 interrupt의미와 다르다. trap
3. exception (0으로 나누려고 했던, 본인에게 권한없는 기계어 명령을 실행하려고 할 떄)
-> 사용자 프로그램이 CPU를 가지고 있다가 스스로 내어 놓던지, 빼앗기던지 CPU가 다른 프로그램, 또는 운영체제에게 넘어가는 경우는?
1. 나는 계속 쓰고 싶은데, timer interrupt에 의해 뺏김
2. 더이상 cpu를 쓸 의지가 없는 경우(오래 걸리는 I/O작업을 만난 경우)

### Device Controller 
#### I/O device controller
- CPU가 컨트롤러에게 하는 기계어가 있다. 모든 I/O장치를 접근하는 = CPU가 I/O를 해달라고 부탁하는 그런 기계어들은 특권 명령으로 묶여있다. 사용자 프로그램이 직접 그 기계어를 실행할 수가 없다. 그게 바로 System call이다. 사용자 프로그램이 뭔가를 하고 싶은데 특권 명령을 하는 기계어다.
- CPU가 어떤 파일을 읽어달라고 하면, disk controller는 헤드를 옮겨서 자기 버퍼에 들여쓰는 작업을 한다. (대단히 오래 걸림).  그 동안 다른 프로그램에 CPU 를 넘김. 다 읽으면 cpu에게 interrupt를 건다. cpu제어권이 운영체제.
읽어온 파일을 메모리 위치에 카피. cpu를 얻으면 본인의 기계어 실행 가능.

- I/O device controller
	- 해당 I/O 장치유형을 관리하는 일종의 작은 CPU
	- 제어 정보를 위해 control register, status register를 가짐
	- local buffer를 가짐(일종의 data register)
- I/O는 실제 device와 local buffer 사이에서 일어남.
- Device controller는 I/O가 끝났을 경우 interrupt로 CPU에 그 사실을 알림.
- device driver(장치구동기)
	- OS코드 중 각 장치별 처리 루틴 ->software
- device controller(장치제어기)
	- 각 장치를 통제한느 일종의 작은 cpu ->hardware
#### 시스템 콜
- 사용자 프로그램이 운영체제의 서비스를 받기 위해 커널 함수를 호출하는 것을 말한다. 
- 프로세스가 직접 programcounter를 넘길 수 없기에, 자신의 기계어를 통해 인터럽트 라인을 셋팅한다. 
  
#### 인터럽트(interrupt)
- 인터럽트를 당한 시점의 레지스터와 program counter를 save한 후 CPU의 제어를 인터럽트 처리 루틴에 넘긴다.
- interrupt(넓은 의미)
	- interrupt(하드웨어 인터럽트): 하드웨어가 발생시킨 인터럽트)
	- Trap(소프트웨어 인터럽트)
		- Exception : 프로그램 오류
		- System call : 프로그램이 커널함수 호출

- 인터럽트 관련 용어
	- 인터럽트 벡터 : 해당 인터럽트의 처리 루틴 주소를 가지고 있음 (인터럽트 종류별로 실행해야 할 코드의 위치를 담고 있음, 주소에 대한 포인터), 타이머가 실행했을 때 그리고 디스크 컨트롤러가 발생시켰을 때 ...
	- 인터럽트 처리 루틴(interrupt service routine, 인터럽트 핸들러)
		- 해당 인터럽트를 처리하는 커널 함수
	
### synchronous I/O, asynchronous I/O
### 동기식 입출력과 비동기식 입출력
- 동기식 입출력(synchronous I/O)
	- I/O 요청 후 입출력 작업이 완료된 후에야 제어가 사용자 프로그램에 넘어감.
	- 구현방법 1
		- I/O가 끝날 때까지 CPU 낭비시킴
		- 매시점 하나의 I/O만 일어날 수 있음
	- 구현방법 2
		- I/O가 완료될 때까지 해당 프로그램에서 CPU 빼았음
		- I/O처리를 기다리는 중 그 프로그램을 줄 세움
		- 다른 프로그램에게 CPU를 줌
- 비동기식 입출력(asynchronous I/O)
	- I/O가 시작된 후 입출력 작업이 끝나기를 기다리지 않고 제어가 사용자 프로그램에 즉시 넘어감.
- 두 경우 모두 I/O 완료는 인터럽트로 알려줌 

- CPU가 I/O요청을 한다?

### DMA(Direct Memory Access)
- 빠른 입출력 장치를 메모리에 가까운 속도로 처리하기 위해 사용
- CPU의 중재 없이 device controller가 device의 buffer storage의 내용을 메모리 block단위로 직접 전송
- 바이트 단위가 아니라 block 단위로 인터럽트를 발생시킴.

### 서로 다른 입출력 기계어
- I/O를 수행하는 special instruction에 의해 (메모리 접근하는 기계어 따로 있고, I/O 수하는 기계어 따로 있고)
- Memory mapped I/O에 의해 (메모리 접근 하는 기계어로 I/O까지 하는 것이다. 메모리 주소가 I/O장치까지 연장해서 매겨짐)

### 저장장치 계층 구조
- 캐싱: 재사용성
 
# [5강. 컴퓨터 시스템 구조] -> 그림 그리며 설명할 수 있어야 한다.
- CPU, Memory, I/O device.
- 프로그램을 실행시키면 프로그램이 메모리에 올라가서 프로세스
- 디스크를 관리하는 작은 cpu는 디스크 컨트롤러 ... (기계어를 연산하는 기능), CPU는 작업공간(메모리)이 필요하다. 컨트롤러 같은 작은 cpu도 작업하는 공간이 필요한데 이를 로컬 버퍼라고 한다.
- CPU가 사용자에게 넘어가면, 그걸 제어할 방법은 없다. CPU에서 기계어를 실행할 떄 사용자가 실행하는 건지, 운영체제가 실행하는 건지 `mode bit`를 가지고 있다. (usermode, kernel mode), 위험한 명령을 '특권명령'이라고도 함.
- mode bit : 사용자 프로그램의 잘못된 수행으로 다른 프로그램 및 운영체제에 피해가 가지 않도록 하기 위한 보호 장치 필요.
- interrupt나 Exception발생 시 하드웨어가 mode bit를 0으로 바꿈.
- 사용자 프로그램에게 CPU를 넘기기 전 mode bit를 1으로 셋팅
- 컴퓨터는 메모리에서 기계어를 읽어와 하나씩 실행한 다음에 다음 기계어를 실행하기에 앞서 interrupt에 signal이 들어온 게 있는지 확인한다.
- CPU가 작업을 하다 Disk를 읽어와야 한다면, 본인이 직접 읽지 않고 Disk Controller한테 어떤 파일좀 읽어줘 하고 부탁을 하면, 컨트롤러는 일을 한다. CPU는 다른 프로그램에게 넘어가게 되고, 디스크 컨트롤러가 일을 다 했으면 CPU한테 알려주는 데, 이를 인터럽트. CPU는 그다음 기계어를 실행 하기에 앞서 인터럽트 들어온 걸 확인. 인터럽트가 들어왔으면 CPU는 자동적으로 운영체제한테 넘어감. 그 인터럽트에 대응하는 일을 한다.
- registers에는 program counter라는 레지스터가 있다. program counter 레지스터는 메모리의 주소를 가지고 있다. 다음번에 실행할 기계어의 메모리를 가지고 있음. 
- 프로그램한테서 CPU를 뺏어오는 건 운영체제 혼자서 할 수가 없다. CPU의 독점을 막기 위해서는 부가적인 하드웨어를 두고 있다. 이를 timer라고 함. timer는 일정 시간 간격으로 인터럽트를 발생 시킴. 운영 체제가 사용자 A프로그램에 CPU를 넘길 때 먼저 timer 시간을 설정함.
- 타이머
	- 정해진 시간이 흐른 뒤 운영체제에게 제어권이 넘어가도록 인터럽트를 발생시킴.
	- 타이머는 매 클럭 틱 때마다 1씩 감소
	- 타이머 값이 0이 되면 인터럽트 발생
	- CPU를 특정 프로그램이 독점하는 것으로부터 보호
- 타이머는 time sharing 구현을 위해 널리 이용됨.
- 타이머는 현재 시간을 계산하기 위해서도 사용.


- 입출력 장치들이 interrupt line 에 
# [3 ~ 4강. 컴퓨터 시스템 구조1 2]
> keyword
```
operating system
time sharing(시분할)
---
realtime(실시간)

multitasking
multiprogramming
multiprocess 
multiprocessor

operating system what to do
```

- 데비안 쉘에서 하나의 명령만 처리하는게 단일 작업?
- 멀티 태스킹과 멀티 스레딩?
- 윈도우는 단일 사용자를 위한 운영체제인가?
- 사용자를 나누는 이유?
- 가상 컴퓨터 만들어보기
	- https://www.coursera.org/learn/build-a-computer?action=enroll
> 새롭게 알게 된 내용
- what is different memory management and process management?

```
Kernel take care of memory management, process management, task management and disk management. 
	- The Process Manager manages processes in the system and is responsible for the sharing of the CPU.
	- The Memory Manager manages memory in the system and is responsible for allocation and deallocation of memory, virtual memory management, etc	
	- Task Management: That is, this function of operating system manages the completion of users' tasks. each task and interrupts the CPU operations to manage tasks efficiently. Task management may involve a multitasking capability. 

The Memory Manager manages memory in the system and is responsible for allocation and deallocation of memory, virtual memory management, etc.
```

- how allocate memory.
```
굉장히 많은 프로그램이 실행 될 때 운영체제는 공평하게 분배하는 게 아니라 일정 프로그램에 집중적으로 메모리를 할당하고, 나머지 프로그램은 swap영역에 있게 된다. 한 프로그램이 충분히 실행되고 난 후(I/O입출력 등) 할당된 자원을 다른 프로그램에 분배한다. 이떄도 대개 집중적으로 메모리를 할당해준다.
```
- 좁은 의미의 운영체제는 커널(운영체제의 가장 핵심적인 부분, 메모리에 항상 상주하는 부분)
- 넓은 의미의 운영체제는 커널 뿐 아니라 각종 주변 시스템 유틸리티 포함하는 개념
- 컴퓨터 시스템의 `자원(resource)을 효율적으로 관리` --> 소프트웨어 자원도 포함된다.
- 컴퓨터의 두뇌는 운영체제가 아닐까? CPU는 단순히 연산이 빠르다. 기억을 하려면 memory가 필요하다. 판단능력은 없기 때문이다.
- 운영체제는 관리하는 역할이기에 판단을 하기에 두뇌역할을 한다. (통치자역할) 
- 누구한테 메모리를, 누구한테 cpu를 얼마나 줄까, 효율적, 공평하게. 
- 메모리 관리 ( 한정된 메모리 공간에 여러 프로그램이 동시에 올라감. 프로그램마다 메모리를 조금씩 나누어져야 한다) 
	- 무조건 공평하게 배분하면 어느 하나가 발전하지 못한다. (사회 지원제도와 비슷하다) 메모리도 이처럼 동작함. 
	- 메모리 관리. 굉장히 큰 많은 프로그램이 실행될 때 메모리 공간을 공평하게 n분의 1로 나누어 쓰는 게 아니라 특정 프로그램은 (swap)부분으로 쫓아내고, 특정 프로그램만 메모리를 많이 주는 경우가 있다. 많이 쓰고 있는 프로그램은 충분히 썼기에 I/O를 하러 떠난다. 그러면 그 친구의 메모리를 뺏어 다른 프로그램에 집중투자함. (몰아주는 게 훨씬 더 효율적)
- 운영 체제의 분류
	1. 동시 작업이 가능한지
		- multi tasking이 가능하다(동시에 2개이상 작업 처리)
	2. 사용자의 수
		- 단일, 다중(좀 더 복잡한 문제, ex - 보안 문제)
	3. 처리방식
		- 일괄처리(batch processing)
			- 작업 요청의 일정량 모아서 한꺼번에 처리
			- 작업이 완전 종료될 떄까지 기다려야함.
			- ex)punch card 처리 시스템
		- 시분할(time sharing)
			- 여러 작업을 수행할 때 컴퓨터 처리 능력을 일정한 시간단위로 분할하여 사용.
			- 일괄 처리 시스템에 비해 짧은 응답 시간을 가짐
			- interactive한 방식
		- 실시간(realtime OS)
			- 정해진 시간 안에 어떠한 일이 반드시 종료됨이 보장되어야 하는 실시간 시스템을 위한 OS
			- ex) 원자로/ 공장 제어, 미사일 제어, 반도체 장비, 로보트 제어
		- 실시간 시스템의 개념 확장
			- Hard realtime system(경성 실시간 시스템)
			- soft realtime system(연성 실시간 시스템)
- Multitasking (컴퓨터 안에 프로그램을 동시에 여러개 돌리는 거)
- Multiprogramming (메모리 측면 강조, 메모리에 여러 프로그램을 동시에 돌리는거)
- time sharing (시간을 쪼개서 쓴다, CPU강조)
- Multiprocess 
  - 다들 똑같은 얘기다. 
- Multiprocessor : 하나의 컴퓨터에 CPU가 여러개 붙어 있음.

### UNIX
- C
- 높은 이식성
- 최소한 커널 구조
- 복잡한 시스템에 맞게 확장 용이
- 오픈 소스
```c
원래는 하드웨어를 제어하고 싶으면 어셈블리어를 사용해야 했다. 효율적인 언어지만 사람이 이해하기는 어렵고, 아예 C언어를 만듬.

C언어 사람이 쓰기 쉽지만, 하드웨어를 다루기도 좋은 언어.
```

### MS_DOS (Disk operating system)
- 개인적 컴퓨터를 위한 컴퓨터. 단일 사용자 운영체제. 메모리 관리 능력 한계(주 기억장치 640kb)

### 운영체제의 구조
- CPU 스케쥴링
- 메모리 관리
- 발열 관리
- 입출력 관리(각기 다른 입출력장치와 컴퓨터간에 어떻게 정보를 주고 받게 하지?)
- 프로세스 관리
	- 프로세스의 생성과 삭제
	- 자원 할당 및 반환
	- 프로세스간 협력
- 그 외 (보호시스템, 네트워킹, 명령어해석기)

- 이 수업을 OS사용자 관점이 아니라 OS 개발자 관점에서 수강해야 한다.
	- 대부분 알고리즘은 OS 프로그램 자체의 내용
	- 인간의 신체가 뇌의 통제 받듯 컴퓨터 하드웨어는 운영체제의 통제를 받으며, 그 운영체제는 사람이 프로그래밍
	- 본인이 window xp나 linux같은 운영체제라고 생각하고 본인이 할 일이 무엇인지를 생각해보면 앞으로 배울 내용을 명확히 알 수 있다.

# [2강. 운영체제 개요2]
> Keyword
```
CPU Scheduling(FCFS, SJF, RR)
Memory Management                 <today>
-------------------------------- 
Disk Scheduling
	- 구성
	- FCFS, SSTF, SCAN
저장장치 계층 구조
캐싱
플래시 메모리
```
> Question
```
가상 메모리는 무엇이고 왜 필요할까?
```
> 가상메모리
```
초기 컴퓨터에선 응용 프로그램의 크기가 사용가능한 RAM크기보다 큰 경우가 많았다. '메모리 부족'을 해결하기 위한 다양한 시도가 있었고, 그 중에는 '오버레이 기법', '가상 메모리 기법'이 있다. 

'오버레이 기법'은 전체 프로그램을 몇개의 모듈로 나누어 컴파일하고, 필요한 모듈만 메모리에 올리는 방식이다. 용량이 큰 프로그램을 부분으로 나눈 뒤 필요할 때마다 이를 꺼내오는 방식인데, 학생 성적을 처리하는 프로그램이라면 (데이터 입력)(데이터 연산)(인쇄) 등의 세그먼트로 나누어 순차적으로 실행되게 한다. (프로그램을 각 모듈로 나누고 그 모듈간의 데이터 교환을 실행시켜야하는 작업을 요한다)

기존에는 프로세스가 실행되는 코드의 전체를 메모리에 로드해야 했고, 메모리 용량보다 더 큰 프로그램은 실행시킬 수 없었다. 하지만 실제로 코드의 일부에서만 대부분의 시간을 사용하고, 프로세스는 특정 순간에는 항상 작은 양의 주소 공간을 사용했기에 이런 방식은 비효율적이었다.

'가상 메모리'란 프로세스를 실행할 때 실행에 필요한 일부만 메모리에 로드하고 나머지는 디스크에 둔다. 이를 통해 프로세스 전체가 물리적 메모리에 있는 것처럼 보인다. 결과적으로 메모리에 작은 양의 주소 공간만 있으면 충분히 프로세스를 수행할 수 있다.

```
> 참고자료
- https://rebro.kr/179

# CPU 스케쥴링
### FCFS (First Come First Served)
- Waiting time
- Average wating time 

### SJF (Shortest-Job-First)
- 금번 cpu 사용시간이 가장 짧은 프로세스를 제일 먼저 스케쥴
- SJF는 `minimum average waiting time`을 보장한다.
- Starvation(기아 현상)발생 가능. (형평성에 문제)

### Round Robin (RR)
- 각 프로세스 동일 크기의 CPU 할당시간을 가짐
- 할당시간이 끝나면 인터럽트가 발생하여 프로세스는 CPU를 빼앗기고 CPU큐의 제일 뒤에 줄을 선다.
- n개의 프로세스가 CPU 큐에 있는 경우
	- 어떤 프로세스도 (n-1)*할당시간 이상 기다리지 않음
	- 대기시간이 프로세스의 CPU 사용시간에 비례

### 메모리 관리
- 프로그램을 실행시키면 그것을 실행시킬 독자적인 메모리 주소가 부여되고 그것이 바로 물리적인 메모리에 올라가는 것이 아니라 본인만의 가상 메모리에 만들어진다. 실제 메모리는 당장 필요한 부분만 가상메모리에서 가져와 사용하게 된다.
- 디스크(파일시스템, 스왑영역)
- 메모리 다 찼을 때 swap영역으로 쫓아낸다. (메모리의 연장 공간이다, 컴퓨터가 꺼지면 메모리 사라짐, swap영역은 살아있지만 의미 없는 정보이다)
- 쪼개지는 단위를 page. 어떤 page를 메모리에서 쫓아낼 것인가?
	- 미래를 모르는 상황에서 다시 사용하지 않을 확률이 높은 페이지를 swap영역으로 쫓아낸다.
	- 미래를 모를 때 과거를 본다.
- LRU(가장 오래 전에 참조 페이지 삭제)
- LFU(참조횟수가 가장 적은 페이지 삭제)

### 디스크 스케쥴링
- 디스크 큐
- 디스크 스케쥴링을 효율적으로 하는 핵심은 디스크 헤드의 이용(디스크 접근 비용 크다)을 최대한 줄이는 것이다.
- 디스크 접근 시간의 구성
	- 탐색시간(seek time)
		- 헤드를 해당 트랙(실린더)으로 움직이는데 걸리는 시간
	- 회전지연(rotation latency)
		- 헤드가 원하는 섹터에 도달하기 전까지 걸리는 시간
	- 전송시간(transfer time)
		- 실제 데이터의 전송 시간
- 디스크 스케쥴링(disk scheduling)
	- seek time을 최소화하는 것이 목표.
	- sekk time ~ seek distance
		- FCFS
		- SSTF(Shortest Seek Time First)
			- stravation 문제
		- SCAN
			- 헤드가 디스크 한쪽 끝에서 다른쪽 끝으로 이동하며 가는 길목에 있는 모든 요청을 처리한다.
			- 다른 한쪽 끝에 도달하면 역방향으로 이동하여 오는 길목에 있는 요청을 처리하며 다시 반대쪽 끝으로 이동한다. (엘리베이터 스케쥴링 scan에 근간)

### 저장장치 계층구조와 캐싱(caching)
- Register
- Cache Memory
- Main Memory      <primary> : CPU직접 접근 가능
--- 
- Magnetic Disk    <secondary>
- Optical Disk
- Magnetic Tape
#### 계층 구조: 속도 차이를 완화하기 위해서.
#### 캐싱(caching): copying information into faster storage system

### 플래시메모리
- 반도체 장치(하드디스크: 마그네틱)
- NAND형(스토리지), NOR형(임베디드 코드 저장용)
- 특징
	- Nonvolatile
	- Low power consumption
	- shock resistance
	- small size
	- Lightweight
	- 쓰기 횟수 제약
		- 골고루 플래시 메모리를 사용할 수 있도록 하게끔(software)
	- 데이터 변질 위험(오랜 시간이 지나면 셀에 최소 전하량 빠지면서 1이란 데이터가 0으로 바뀔 수 있음)
		- 일정시간이 지나면 전하량을 다시 채워주기 위에 옮겨쓰기를 한다던지(software)
- 플래시 메모리의 사용 형태
	- 휴대폰, PDA 등 임베디드 시스템 구성용
	- USB용 메모리 스틱
	- 디지털카메라 등의 SD카드, CompactFlash, Smart Mediacard
	- 모바일 장치 뿐 아니라 대용량 시스템에서 SSD(Solid State Drive)란 이름으로 하드디스크 대체 시도

### 운영체제의 종류 
#### 오픈소스 개발자들은 어떻게 먹고 사는가?
```c
소프트웨어 상품의 특이한 성질.
만드는데 인건비가 드나, 추가적으로 파는 비용은 거의 없다.
독점체제가 가능한 시장.
사실 공개소프트웨어 개발자는 2등이었던 것.
소프트웨어 교육시장, 컨설팅, 명예, 버그를 잡으며 더 탄탄한 프로그램.
지금은 소프트웨어의 특수성이 오히려 강력한 무기가 되는.
```

# [1강. 운영체제 개요]

> keyword
```
운영체제
운영체제의 목적
CPU
컴퓨터 시스템 구조
운영체제 기능
인터럽트
캐싱
```

# 운영체제(Operating System, OS)란?
- 컴퓨터 하드웨어 바로 위에 설치되어 사용자 및 다른 모든 소프트웨어와 하드웨어를 연결하는 소프트웨어 계층
```
[사용자][사용자]
[각종 소프트웨어][사용자]
[운영체제]
[컴퓨터하드웨어]
```

# 운영체제의 목적
- 컴퓨터 시스템을 편리하게 사용할 수 있는 환경을 제공
	- 운영체제는 동시 사용자/프로그램들이 각각 독자적 컴퓨터에서 수행되는 것 같은 환상(illusion)을 제공
	- 하드웨어를 직접 다루는 복잡한 부분을 운영체제가 대행
- 컴퓨터 시스템의 `자원(resource)을 효율적으로 관리` --> 소프트웨어 자원도 포함된다.
	- `CPU, 메모리, I/O 장치` 등의 효율적 관리
		- 주어진 자원으로 최대한의 성능을 내도록 -> `효율성`
		- 특정 사용자/프로그램의 지나친 불이익이 발생하지 않도록 -> `형평성`
	- 사용자 및 운영체제 자신의 보호

```실행중인 프로그램들에게 짧은 시간의 CPU를 번갈아 할당```
```개별 프로세스는 자신이 CPU를 독점하고 있다고 생각```
```클라이언트는 서버를 자기가 혼자 사용하고 있다고 생각```
```CPU 연산속도가 매우 빠르기 때문에 컴퓨터 두뇌```
```기가 헤르츠, 초당 클럭이 10억. 1초에 기계어를 10억개를```
```0.1초, 0.01초 차이를 사람을 느끼지 못한다. CPU는 여러 프로그램들을 0.1초 이내로 번갈아가며 실행을 하면 사람은 인지하기 어렵다```
```컴퓨터 메모리. 실행중인 프로그램들에 메모리 공간을 적절히 분배```

# 컴퓨터 시스템 구조
[컴퓨터 내부]           [입출력I/O 디바이스]
<CPU>                        <디스크>         input/output device
							 <키보드, 마우스> input device
                             <모니터, 프린트> output device
	<메모리>
    
```컴퓨터 전원을 켜면(부팅) 운영체제가 시스템 메모리에 올라간다```
```운영체제란 컴퓨터가 꺼지기 전까지 항상 실행되는 프로그램이다```
```운영체제 중에서 핵심이 되는 부분을 커널이라고 한다```
```cpu의 작업공간을 메모리공간이다```
```cpu는 매 순간(매 클럭) 메모리 어딘가에 있는 기계어를 읽어다가 연산을 한다```
```cpu가 직접 외부장치를 접근할 수 없다```
```cpu가 I/O device들에게 작은 cpu가 붙어있다. I/O Controller다. cpu가 I/O Controller에게 file을 읽어달라고 요청, controller는 요청된 file을 읽고 작은 메모리 공간에 읽게된다. cpu는 그 메모리를 읽는다``` 

```

### 운영체제의 기능
1. CPU 스케쥴링
	- 어떤 프로그램에게 CPU사용권을 줄까?

2. 메모리 관리
	- 한정된 메모리를 어떻게 쪼개어 쓰지?

3. 디스크 스케쥴링
	- 디스크에 들어온 요청을 어떤 순서로 처리할까?

짧은 CPU와 느린 I/O 장치간 속도 차이를 어떻게 극복하지?
(인터럽트, 캐싱)

인터럽트는 느린 장치에다 무언가 일을 시켜놓고, (게속 일을 끝났는지 확인하는 것은 비효율적)
CPU는 자기 할 일을 하고 있고, 느린 장치들에게 시킨 일이 끝나면 CPU에게 (CPU입장에서는 아주 가끔) 인터럽트를 걸어서
처리 사실을 알려주고 CPU의 제어권이 인터럽트에 의해서 자동으로 운영체제에 넘어가서 인터럽트 이후에 해당하는 일을 처리함.

CPU - 기계어 실행

