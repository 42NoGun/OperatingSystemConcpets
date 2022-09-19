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

# [27강. 가상메모리 I]
### Demand Paging
- 실제로 필요할 때 page를 메모리에 올리는 것
	- I/O양의 감소
	- Memory 사용량 감소
	- 빠른 응답 시간
	- 더 많은 사용자 수용
- Valid / Invalid bit의 사용
	- Invaild의 의미
		- 사용되지 않는 주소 영역인 경우
		- 페이지가 물리적 메모리에 없는 경우
	- 처음에는 모든 page entry가 invalid로 초기화
	- address translation 시에 invalid bit이 set되어 있으면 => page fault

### Page Fault
- invalid page를 접근하면 MMU가 trap을 발생시킴(page fault trap)
- Kernel mode로 들어가서 page fault handler가 invoke됨
- 다음과 같은 순서로 page fault를 처리한다.
	- 1. Invalid reference? (eg. bad address, protection violation) => abort process
	- 2. Get an empty page frame. (없으면 뺏어온다:replace)
	- 3. 해당 페이지를 disk에서 memory로 읽어온다.
		1. disk I/O가 끝나기까지 이 프로세스는 CPU를 preempt 당함(block)
		2. Disk read가 끝나면 page tables entry 기록, valid/invalid bit = "valid"
		3. read queue에 process를 insert -> dispatch later
	- 4. 이 프로세스를 CPU를 잡고 다시 running
	- 5. 아까 중단되었던 instruction을 재개

### Performance of Demand Paging
- Page Fault Ratio
	- if p = 0 no page fault
	- if p = 1, every reference is a fault
- effective access time
	- (1 - p) * memory access
		+ p (OS & HW page fault overhead
		+ [swap page out if needed]
		+ swap page in
		+ OS & HW restart overhead)

### Free frame이 없는 경우
- Page replacement
	- 어떤 frame을 빼앗아올지 결정해야 함
	- 곧바로 사용되지 않을 page를 쫓아내는 것이 좋음
	- 동일한 페이지가 여러 번 메모리에서 쫓겨났다가 다시 들어올 수 있음
- Replacement algorithm
	- page-fault rate를 최소화하는 것이 목표
	- 알고리즘의 평가
		- 주어진 page reference string에 대해 page falut를 얼마나 내는지 조사
		- reference string의 예
			- 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5

# [26강. 메모라관리 III]
### Memory protection
- page table의 각 entry마다 아래의 bit를 둔다.
	- protection bit
		- page에 대한 접근 권한(read/write/read-only)
	- valid-invalid bit
		- valid는 해당 주소의 frame에 그 프로세스를 구성하는 유효한 내용이 있음을 뜻함(접근 허용)
		- invalid는 해당 주소의frame에 유효한 내용이 없음을 뜻함(접근 불허)

### Inverted Page Table
- page table이 매우 큰 이유
	- 모든 process 별로 그 logical address에 대응하는 모든 page에 대해 page table entry가 존재
	- 대응하는 page가 메모리에 있든 아니든 간에 page table 에는 entry로 존재
- inverted page table
	- page frame 하나당 page table에 하나의 entry를 둔 것(system-wide)
	- 각 page table entry는 각각의 물리적 메모리의 page frame이 담고 있는 내용 표시(process-id, process의 logical address)
	- 단점
		- 테이블 전체를 탐색해야 함
	- 조치
		- associative register 사용 (expensive)

### Shared Page
- Shared code
	- Re-entrant code(=pure code)
	- read-only로 하여 프로세스 간에 하나의 code만 메모리에 올림(eg, text editors, compilers, window systems)
	- shared code는 모든 프로세스의 logical address space에서 동일한 위치에 있어야함.(page번호가 같아야 한다=> 이 챕터 초반에 ... 주소 바인딩 그림)
- Private code and data
	- 각 프로세스들은 독자적으로 메모리에 올림
	- private data는 logical address space의 아무 곳에 와도 무방

### Segmentation
- 프로그램은 의미 단위인 여러 개의 segment로 구성
	- 작게는 프로그램을 구성하는 함수 하나하나를 세그먼트로 정의
	- 크게는 프로그램 전체를 하나의 세그먼트로 정의 가능
	- 일반적으로 code, data, stack 부분이 하나씩의 세그먼트로 정의됨
- Segment는 다음과 같은 logical unit  들임.
	- main()
	- function,
	- gloval variables,
	- stack,
	- symbol table, arrays

### Segmentation Architecture
- Logical address는 다음의 두 가지로 구성
	- <segment-number, offset>
- Segment table
	- each table entry has
		- base - starting physical address of the segment
		- limit - length of the segment
	- segmentation-tables base register(STBR)
		- 물리적 메모리에서의 segment table의 위치
	- segment-table length register (STLR)
		- 프로그램이 사용하는 segment의 수
				- segment number is legal if s < STLR

### Segmentation Architecture
- Protection
	- 각 세그먼트 별로 protection bit가 있음
	- Each entry:
		- valid bit = 0 -> illegal segment
		- Read/Write/Execution 권한 bit
- Sharing
	- shared segment
	- same segment number
	cf. segment는 의미 단위이기 때문에 공유(sharing)와 보안(protection)에 있어 paging보다 훨씬 효과적이다.
- Allocation
	- first fit / best fit
	- external fragmentation 발생
	- segment의 길이가 동일하지 않으므로 가변분할 방식에서와 동일한 문제점들이 발생

### segmentation with paging
- pure segmentation과의 차이점
	 - segment-table entry가 segment의 base address를 가지고 있는 것이 아니라 segment를 구성하는 page table의 base address를 가지고 있음.

# [25강. 메모라괸리 II]
### Paging
- Paging
	- Process의 virtual memory를 동일한 사이즈의 page단위로 나눔
	- Virtual memory의 내용이 page 단위로 noncontiguous하게 저장됨
	- 일부는 backing storage에, 일부는 physical memory에 저장
- Basic Method
	- physical memory를 동일한 크기의 frame으로 나눔
	- logical memory를 동일 크기의 page로 나눔(frame과 같은 크기)
	- 모든 가용 frame들을 관리
	- page table을 사용하여 logical address를 physical address로 변환
	- External fragmentation 발생 안함
	- Internal fragmentation 발생 가능
- 4GB를 4KB(페이지)나누면 1MB(백만개), 즉 page table도 엔트리가 100만개.- 그럼 32비트 주소에서, 페이지 번호와 오프셋은 몇비트일까? 

### Implementation of Page Table
- Page table은 main memory에 상주
- Page-table base register(PTBR)가 page table을 가리킴
- Page-table length register(PLTR)가 테이블 크기를 보관
- 모든 메모리 접근 연산에는 2번의 memory access 필요
- page table 접근 1번, 실제 data/instruction 접근 1번
- 속도 향상을 위해 associative register 혹은 translation look-aside buffer(TLB)라 불리는 고속의 lookup hardware cache 사용
cf. 캐쉬메모리가 두가지가 있다. 데이터를 위한 캐쉬메모리와 주소변환을 위한 캐쉬메모리. (주소 변환을 빠르게하는 목적인 TLB) 

### Associative Register
- Associative register (TLB) parallel search가 가능
	- TLB에는 page table 중 일부만 존재
- Address translation
	- page table 중 일부가 associative register에 보관되어 있음
	- 만약 해당 page #가 associative register에 있는 경우 곧바로 frame #을 얻음
	- 그렇지 않은 경우 main memory에 있는 page table로부터 frame #를 얻음
	- TLB는 context switch 때 flush(remove old entries)

### effective Access time
- Associative register lookup time = { (입실론)
- memory cycle time = 1
- Hit ratio = a
	- associative register에서 찾아지는 비율
- effective access time (EAT)
	 <hit>		<miss>
EAT= (1 + { )a  + (2 + })(1 - a)
   = 2 + { - a
   
### Two-Level Page Table
- 현대의 컴퓨터는 address space가 매우 큰 프로그램을 지원
	- 32bit address 사용시 :2^32(4G)의 주소공간
		- page size가 4K시 1M개의 page table entry 필요
		- 각 page entry가 4B시 프로세스당 4M의 Page table 필요
		- 그러나, 대부분의 프로그램은 4G의 주소 공간 중 지극히 일부분만 사용하므로 Page table 공간이 심하게 낭비됨
	-> page table 자체를 page로 구성
	-> 사용되지 않는 주소 공간에 대한 outer page table의 엔트리 값은 NULL(대응하는 inner page table이 없음) 

### Two-Level Paging Example
- logical address (on 32-bit machine with 4K page size)의 구성
	- 20 bit의 page number
	- 12 bit의 page offset
- page table 자체가 page로 구성되기 때문에 page number는 다음과 같이 나뉜다(각 page table entry가 4B)
	- 10-bit의 page number
	- 10-bit의 page offset
- 따라서, logical address는 다음과 같다
	page number | page offset
	10  	10		12
- p1은 outer page table의 index이고,
- p2는 outer page table의 page에서의 변위(displacement)

### Multilevel Paging and Performance
- Address space가 더 커지면 다단계 페이지 테이블 필요
- 각 단계의 페이지 테이블이 메모리에 존재하므로 logical address의 physical addressㅂ 변환에 더 많은 메모리 접근 필요
- TLB를 통해 메모리 접근 시간을 줄일 수 있음
- 4단계 페이지 테이블을 사용하는 경우
	- 메모리 접근 시간이  100ns, TLB 접근 시간이 20ns이고
	- TLB hit ratio가 98%인 경우
		- effective memory access time = 0.98 * 120 + 0.02 * 520 = 128nanoseconds
		- 결과적으로 주소 변환을 위해 28ns만 소요

# [24강. 메모리관리 I]
### Logical vs Physical Address
- Logical address(virtual address)
	- 프로세스마다 독립적으로 가지는 주소 공간
	- 각 프로세스마다 0번지부터 시작
	- CPU가 보는 주소는 logical address임
- Physical address
	- 메모리에 실제 올라가는 위치
- 주소 바인딩: 주소를 결정하는 것
	 - symbolic address -> logical address -> physical address

### Compile time binding
- 물리적 메모리 주소(physical address)가 컴파일 시 알려짐
- 시작 위치 변경시 재컴파일
- 컴파일러는 절대 코드(absolute code)생성

### Load time binding
- Loader의 책임 하에 물리적 메모리 주소 부여
- 컴파일러가 재배치가능코드(relocatable code)를 생성한 경우 가능

### Execution time binding(=Run time binding)
- 수행이 시작된 이후에도 프로세스의 메모리 상 위치를 옮길 수 있음
- CPU가 주소를 참조할 때마다 binding을 점검 (address mapping table)
- 하드웨어적인 지원이 필요(e.g base and limit registers. MMU)
cf. CPU가 바라보는 주소는 물리적인 주소일까? 논리적인 주소일까?
cpu기계어안에 들어간 주소가 논리적인 주소니까!
cf. 메모리 주소를 변환하는 일은 운영체제가 하는 일이 아니다. 하드웨어가 하는 일이다. 운영체제가 할려면 자기가 CPU를 잡아서 본인의 코드 실행을 통해서 일을 하는 건데, 메모리 한번 접근하자고 사용자프로그램이 운영체제를 부른다는 건 말이 안됨.

### Memory-Management Unit (MMU)
- MMU
	- logical address를 physical address로 매핑해주는 Hardware device
- MMU scheme
	- 사용자 프로세스가 CPU에서 수행되며 생성해내는 모든 주소값에 대해 base register(=relocation register)의 값을 더한다.
- user program
	- logical address만을 다룬다.
	- 실제 physical address를 볼 수 없으며 알 필요가 없다.

### Hardware Support for Address Translation
- 운영체제 및 사용자 프로세스 간의 메모리 보호를 위해 사용되는 레지스터
	- Relocation register(=base register) : 접근할 수 있는 물리적 메모리 주소의 최소값
	- Limit register : 논리적 주소의 범위

### Dynamic Loading
- 프로세스 전체를 메모리에 미리 다 올리는 것이 아니라 해당 루틴이 불려질 때 메모리에 load하는 것
- memory utilization의 향상
- 가끔씩 사용되는 많은 양의 코드의 경우 유용
- 운영체제의 특별한 지원 없이 프로그램 자체에서 구현 가능(OS는 라이브러리를 통해 지원 가능) cf. 오리지널 다이나믹 로딩. 현재는 운영체제가 올리는 것도 다이나믹 로딩이라고 함.
*Loading: 메모리로 올리는 것

### Overlays //프로그래머 책임으로 ... 메모리가 굉장히 작던 시절
- 메모리에 프로세스의 부분 중 실제 필요한 정보만을 올림
- 프로세스의 크기가 메모리보다 클 때 유용
- 운영체제의 지원없이 사용자에 의해 구현
- 작은 공간의 메모리를 사용하던 초창기 시스템에서 수작업으로 프로그래머가 구현
	- Manual Overlay
	- 프로그래밍이 매우 복잡

### Swapping
- 프로세스를 일시적으로 메모리에서 backing store로 쫓아내는 것
- Backing store(=swap area)
	- 디스크 : 많은 사용자의 프로세스 이미지를 담을 만큼 충분히 빠르고 큰 저장공간
- Swap in / Swap out
	- 일반적으로 중기 스케줄러(swapper)에 의해 swap out 시킬 프로세스 선정
	- priority-based CPU scheduling algorithm
		- priority가 낮은 프로세스를 swapped out시킴
		- priority가 높은 프로세스를 메모리에 올려 놓음
	- Compile time 혹은 load time binding에서는 원래 메모리 위치로 swap in
	- Execution time binding에서는 추후 빈 메모리 영역 아무 곳에나 올릴 수 있음.  (wapping이 잘 지원되려면 runtime binding이 잘 지원되어야 한다.)
	- swap time은 대부분 transfer time(swap 되는 양에 비례하는 시간)임
		(원래는 diskhead시간이 대부분이었는데, 여기에선 이례적으로(그 이유는 양이 너무 많아서) transfer time이 대부분)

### Dynamic Linking
- Linking을 실행 시간(execution time)까지 미루는 기법
- Static linking (static library)
	- 라이브러리가 프로그램의 실행 파일 코드에 포함됨
	- 실행 파일의 크기가 커짐
	- 동일한 라이브러리를 각각의 프로세스가 메모리에 올리므로 메모리 낭비(printf 함수의 라이브러리 코드)
(라이브러리가 내 실행파일 안에 포함되어 있다-> 스태틱)
- Dynamic linking (shared library) shared object (.so .dll)
	- 라이브러리가 실행시 연결(link)됨
	- 라이브러리 호출 부분에 라이브러리 루틴의 위치를 찾기 위한 stub라는 작은 코드를 둠
	- 라이브러리가 이미 메모리에 있으면 그 루틴의 주소로 가고 없으면 디스크에서 읽어옴
	- 운영체제의 도움이 필요

### Allocation of Physical Memory
- 메모리는 일반적으로 두 영역으로 나뉘어 사용
	- OS 상주 영역
		- interrupt vector와 함께 낮은 주소 영역 사용
	- 사용자 프로세스 영역
		- 높은 주소 영역 사용
- 사용자 프로세스 영역의 할당 방법
	- contiguous allocation (연속 할당)
		- 각각의 프로세스가 메모리의 연속적인 공간에 적재되도록 하는 것
		- Fixed partition allocation(고정분할 방식)
			- 물리적 메모리를 몇 개의 영구적 분할(partition)로 나눔)
			- 분할의 크기가 모두 동일한 방식과 서로 다른 방식이 존재
			- 분할당 하나의 프로그램 적재
			- 융통성이 없음
				- 동시에 메모리에 load되는 프로그램의 수가 고정됨
				- 최대 수행 가능 프로그램 크기 제한
			- Internal fragmentation 발생 (external fragmentation도 발생)
		- Variable partition allocation(가변 분할 방식)
			- 프로그램의 크기를 고려해서 할당
			- 분할의 크기, 개수가 동적으로 변함
			- 기술적 관리 기법 필요
			- External fragmentation 발생
	- NonContiguous allocation (불연속 할당)
		- 하나의 프로세스가 메모리의 여러 영역에 분산되어 올라갈 수 있음
		- Paging
		- Segmentation
		- Paged Segmentation

- 외부조각(External fragmentation)
	- 프로그램 크기보다 분할의 크기가 작은 경우
	- 아무 프로그램에도 배정되지 않은 빈 곳인데도 프로그램이 올라갈 수 없는 작은 분할
- 내부조각(Internal fragmentation)
	- 프로그램 크기보다 분할의 크기가 큰 경우
	- 하나의 분할 내부에서 발생하는 사용되지 않는 메모리 조각
	- 특정 프로그램에 배정되었지만 사용되지 않는 공간
- Hole
	- 가용 메모리 공간
	- 다양한 크기의 hole들이 메모리 여러 곳에 흩어져 있음
	- 프로세스가 도착하면 수용가능한 hole을 할당
	- 운영체제는 다음의 정보를 유지
		- a)할당 공간 b) 가용 공간(hole)
### Dynamic Storage-Allocation Problem
- 가변 분할 방식에서 size n인 요청을 만족하는 가장 적절한 hole을 찾는 문제
	- First-fit
		- Size가 n 이상인 것 중 최초로 찾아지는 hole에 할당
	- Best-fit
		- Size가 n이상인 가장 작은 hole을 찾아서 할당
		- Hole들의 리스트가 크기순으로 정렬되지 않은 경우 모든 hole의 리스트를 탐색해야함
		- 많은 수의 아주 작은 hole들이 생성됨
	- Worst-fit
		- 가장 큰 hole에 할당
		- 역시 모든 리스트를 탐색해야 함
		- 상대적으로 아주 큰 hole들이 생성됨
- First-fit과 best-fit이 worst-fit보다 속도와 공간 이용률 측면에서 효과적인 것으로 알려짐(실험적인 결과)

### compaction
- external fragmentation 문제를 해결하는 한 가지 방법
- 사용 중인 메모리 영역을 한군데로 몰고 hole들을 다른 한 곳으로 몰아 큰 block을 만드는 것
- 매우 비용이 많이 드는 방법임
- 최소한의 메모리 이동으로 compaction하는 방법(매우 복잡)
- Compaction은 프로세스의 주소가 실행 시간에 동적으로 재배치 가능한 경우에만 수행될 수 있다.
# [23강. 데드락]
### Deadlock Avoidance 
- 시스템이 safe state에 있으면
	-> no deadlock
- 시스템이 unsafe state에 있으면
	-> possibility of deadlock
- Deadlock Avoidance
	- 시스템이 unsafe state에 들어가지 않는 것을 보장
	- 2가지 경우의 avoidance 알고리즘
		- Single instance per resouce types
			- Resource Allocation Graph algorithm
		- Multiple instances per resource types
			- Banker's Algorithm
### Banker's algorithm
- 자원의 여유가 있더라도, 혹시 자원을 최대한 요청했을 떄 그 요청을 여유있는 가용자원으로 처리하지 못한다면 데드락이 생길 수 있으므로, 그 요청을 받아들이지 않는다. (가용자원만 가지고 판단)

### Deadlock Detection 
- Deadlock Detection
	- Resource type당 single instance인 경우
		- 자원할당 그래프에서의 cycle이 곧 deadlock을 의미
	- Resource type 당 multiple instance인 경우
		- banker's algorithm과 유사한 방법 활용
- wait for graph 알고리즘
	- Resource type당 single instance인 경우
	- wait-for graph
		- 자원할당 그래프의 변형
		- 프로세스만으로 node 구성
		- Pi가 가지고 있는 자원을 Pj가 기다리는 경우 pj->pi
	- Algorithm
		- Wait-for graph에 사이클이 존재하는지를 주기적으로 조사
- 굉장히 낙관적으로 보는 알고리즘이고, 자원 반납이 완료가능한 프로세스의 자원을 합하더라도, 어떤 요청을 처리해서 추가적인 반납을 할 수 없는 경우에 데드락으로 본다.
 
### Recovery
- Process termination
	- About all deadlocked process
	- Abort one process at a time until the deadlock cycle is eliminated- Resource Preemption
	- 비용을 최소화할 victime의 선정
	- safe state로 rollback하여 process를 restart
	- starvation 문제
		- 동일한 프로세스가 계속해서 victim으로 선정되는 경우
		- const factor에 rollback 횟수도 같이 고려

### Deadlock Ignorance
- Deadlock이 일어나지 않는다고 생각하고 아무런 조치도 취하지 않음
	- Deadlock이 매우 드물게 발생하므로 Deadlock에 대한 조치 자체가 더 큰 overhead일 수 있음
	- 만약, 시스템에 deadlock이 발생한 경우 시스템이 비정상적으로 작동하는 것을 사람이 느낀 후 직접 process를 죽이는 등의 방법으로 대처
	- UNIX, Windows 등 대부분의 범용 OS가 채택

# [22강. 병행제어 III 3]
### 교착상태(deadlock)
### The Deadlock Problem
- Deadlock
	- 일련의 프로세스들이 서로가 가진 자원을 기다리며 block된 상태
- Resource(자원)
	- 하드웨어, 소프트웨어 등을 포함하는 개념
	- ex) I/O device, CPU cycle, memory space, semaphore 등
	- 프로세스가 자원을 사용하는 절차
		- Request, Allocate, Use, Release
	- Deadlock Example 1
		- 시스템에 2개의 tape drive가 있다.
		- 프로세스 P1과 P2 각각이 하나의 tape drive를 보유한 채 다른 하나를 기다리고 있다.
	- Deadlock Example 2
		- Binary semaphores A and B

### Deadlock 발생의 4가지 조건
- Mutual exclusion
	- 매 순간 하나의 프로세스만이 자원을 사용할 수 있음
- No preemption
	- 프로세스는 자원을 스스로 내어놓을 뿐 강제로 빼앗기지 않음
- Hold and wait
	- 자원을 가진 프로세스가 다른 자원을 기다릴 때 보유 자원을 놓치 않고 계속 가지고 있음
- Circular wait
	- 자원을 기다리는 프로세스간에 사이클이 형성되어야 함
	- 프로세스 P0, P1, ..., Pn이 있을 때
		- p0은 p1이 가진 자원을 기다림
		- p1은 p2가 가진 자원을 기다림
		- p(n-1)은 p(n)이 가진 자원을 기다림
		- pn 은 p0을 가진 자원을 기다림.
		 
### 자원 할당 그래프

### Deadlock의 처리 방법
- Deadlock prevention
	- 자원 할당 시 deadlock의 4가지 필요 조건 중 어느 하나가 만족되지 않도록 하는 것
- Deadlock avoidance
	- 자원 요청에 대한 부가적인 정보를 이용해서 deadlock의 가능성이 없는 경우에만 자원을 할당
	- 시스템 state가 원래 state로 돌아올 수 있는 경우에만 자원 할당
- Deadlock Detection and recovery
	- Deadlock 발생은 허용하되 그에 대한 detection 루틴을 두어 deadlock 발견시 recover
- Deadlock Ignorance
	- Deadlock을 시스템이 책임지지 않음
	- UNIX를 포함한 대부분의OS가 채

### Deadlock Prevention
- Mutual Exclusion
	- 공유해서는 안되는 자원의 경우 반드시 성립해야함
- Hold and wait
	- 프로세스가 자원을 요청할 때 다른 어떤 자원도 가지고 있지 않아야 한다.
	- 방법1. 프로세스 시작 시 모든 필요한 자원을 할당받게 하는 방법
	- 방법2. 자원이 필요할 경우 보유 자원을 모두 놓고 다시 요청
- No preemption
	- process가 어떤 자원을 기다려야 하는 경우 이미 보유한 자원이 선점됨
	- 모든 필요한 자원을 얻을 수 있을 때 그 프로세스는 다시 시작된다.
	- State를 쉽게 save하고, restore할 수 있는 자원에서 주로 사용(CPU, memory)
- Circular Wait
	- 모든 자원 유형에 할당 순서를 정하여 정해진 순서대로만 자원 할당
	- 예를 들어 순서가 3인 자원 R1를 보유 중인 프로세스가 순서가 1인 자원 Rj를 할당받기 위해서는 우선 R1을 release해야 한다.
-> Utilization 저하, throughput 감소, starvation 문제

### Deadlock Avoidance
- 자원 요청에 대한 부가정보를 이용해서 자원 할당이 deadlock으로부터 안전한지를 동적으로 조사해서 안전한 경우에만 할당
- 가장 단순하고 일반적인 모델은 프로세스들이 필요로 하는 각 자원별 최대 사용량을 미리 선언하도록 하는 방법임
- safe sate
	- 시스템 내의 프로세스들에 대한 safe sequence가 존재하는 상태
- safe sequence 
  	- 프로세스의 sequence<P1, P2, ... Pn>이 safe하려면 pI(1<=i<=n)의 자원 요청이 `가용 자원 + 모든 Pj (j < i/)의 보유 자원`에 의해 충족되어야 함.
	- 조건을 만족하면 다음 방법으로 모든 프로세스의 수행을 보장
		- P1의 자원요청이 즉시 충족될 수 없으면 모든pj (i < j)가 종료될 때까지 기다린다.
		- pi-1이 종료되면 pi의 자원요청을 만족시켜 수행한다.

### Resource Allocation Graph algorithm
<사진> 33:44

> keyword
```
monitor
	- monitorㄹ동작 원리
	- semaphore의 문제점
- Bounded-buffer
- phillosopher
```
# [21강. 병행제어 II 2]
### monitor
- Semaphore의 문제점
	- 코딩하기 힘들다
	- 정확성(correctness)의 입증이 어렵다
	- 자발적 협력(voluntary cooperation)이 필요하다
	- 한번의 실수가 모든 시스템에 치명적 영향
- 동시 수행중인 프로세스 사이에서 abstract data type의 안전한 공유를 보장하기 위한 high-level synchronization construct
```
monitor monitor-name
{	//shared variable declarations
	procedure body P1(...) {...}
	procedure body P2(...) {...}
	procedure body P3(...) {...}
- 모니터는 공유데이터 접근은 모니터 안에서 정의한 함수에 한해서 가능.
}
```
- 모니터 내에서는 한번에 하나의 프로세스만이 활동 가능
- 프로그래머가 동기화 제약 조건을 명시적으로 코딩할 필요없음
- 프로세스가 모니터 안에서 기다릴 수 있도록 하기 위해
	- condition variable 사용 -> condition x,y:
- condition variable은 wait와 signal연산에 의해서만 접근 가능 x.wait()
	- x.wait()
		- 을 invoke한 프로세스는 다른 프로세스가 x.signal()을 invoke하기 전까지 suspend된다.
	- x.signal()
		- 은 정확하게 하나의 suspend된 프로세스를 resume한다. suspend된 프로세스가 없으면 아무 일도 일어나지 않는다.
### Bounded-buffer problem
```c
monitor bounded_buffer
{
	int buffer[N];
	condition full, empty;
	/* condition var.은 값을 가지지 않고 자신의 큐에 프로세스를 매달아서 sleep 시키거나 큐에서 프로세스를 깨우는 역할만 함*/
	void produce(int x)
	{
		if there is no empty buffer
			empty.wait()
		add x to an empty buffer
		full.signal(); // 세마포어와 달리 잠들어있는 친구 없으면 그냥 아무일도 일어나지 않음
	}
	void consume (int *x)
	{
		if there is no full buffer
			full.wait();
		remove an item form buffer and store it to *x
		empty.signal();
	}
}
```
### Dining Philloshphers example
```c
monitor dining_philosopher
{
	enum {thinking, hungry, eating) state[5];
	condition self[5];
	void pickup(int i)
	{
		state[i] = hungry;
		test(i);
		if (state[i] != eating)
			self[i].wait();
	}
	
	void putdown(int i)
	{
		state[i] = thinking;
		/* test left and right neighbors */
		test((i+4) % 5);
		test((i+1) % 5);
	}
	
	void test(int i)
	{
		if ((state[(i + 4) % 5] != eating) && 
		(state[i] == hungry) && (state[(i + 1) % 5] != eating))
		{
			state[i] = eating;
			self[i].signal();
		}
	}
	
	void init() {
	for (int i = 0; i < 5; ++i)
		state[i] = thinking;
	}
	
	Each phillosopher:
	{
		pickup(i);
		eat();
		putdown(i);
		think();
	} while (1);
```



> keyword
```
- Deadlock
- Bounded-buffer problem
- Reader and writer
- phillosopher
```
# [20강. 병행제어 II 1]
### Deadlock and Starvation
- Deadlock: 둘 이상의 프로세스가 서로 상대방에 의해 충족될 수 있는 event를 무한히 기다리는 현상
- S와 Q가 1로 초기화된 semaphore라 하자. //2개의 자원을 모두 획득해야 일 가능하다면. 일이 다 끝내야 자원을 내놓는구조이므로. 자원을 얻는 순서를 일치시키면 데드락X
	- P(S);			P(Q);	하나씩 차지
	- P(Q);			P(S);	상대방 것을 요구
	- ...			...
	- V(S);			V(Q);	여기와야 release
- Starvation: indefinite blocking. 프로세스가 suspend된 이유에 해당하는 세마포어 큐에서 빠져나갈 수 없는 현상

### Classical problems of Synchronization
- Bounded-Buffer Problem(Producer-consumer Problem)
	- 공유버퍼에 락을 걸로 푸는.
	- 유한한 버퍼이므로, 생산자 입장에서는 비어있는 버퍼가 있어야..(빈 버퍼가 자원)
	- 소비자 입장에서는 버퍼가 채워져 있어야.. (내용이 들어있는 버퍼가 자원)
	- mutual exclusion -> Need binary semaphore(shared data의 mutual exclusion을 위해)
	- resource count -> Need integer semaphore(남은 full/empty buffer의 수 표시)
	- producer
```c
do {
	produce an item in x
	...
	P(empty);
	P(mutex);
	...
	add x to buffer
	...
	V(mutex);
	V(full);
```
	- consumer
```c
do {
	P(full)
	P(mutex);
	...
	remove an item from buffer to y
	...
	V(mutex);
	V(empty);
	...
	consume the item in y
	...
	} while (1);
```
- Readers and Writers Problem
	- 한 process가 DB에 write중일 때 다른 process가 접근하면 안됨
	- read는 동시에 여럿이 해도 됨
	- solution
		- writer가 DB에 접근 허가를 아직 얻지 못한 상태에서는 모든
		대기중인 Reader들을 다 DB에 접근하게 해준다.
		- writer는 대기 중인 Reader가 하나도 없을 때 DB접근이 허용된다.
		- 일단 writer가 DB에 접근 중이면 Reader들은 접근이 금지된다
		- writer가 DB에서 빠져나가야만 Reader의 접근이 허용된다.
	- Shared data
		- DB자체
		- readcount //현재 DB에 접근 중인 Reader의 수
	- Synchronization variable
		- mutex : 공유 변수 readcount를 접근하는 코드(critical section)의 mutual exclusion 보장을 위해 사용.
		- db : Reader와 writer가 공유 DB자체를 올바르게 접근하게 하는 역할
```c
int readcount = 0;
DB자체;
Synchronization variable
semaphore mutex = 1, db = 1;

	Writer
	p(db);
	...
	waiting DB is performed
	...	
	V(db);
	
	Reader;
	p(mutex);
	readcount++;
	if (readcount == 1) p(db);// block writer
	V(mutex);
	...
	reading DB is performed
	...
	P(mutex)
	readcount--;
	if (readcount == 0) V(db); //enable writer
	V(mutex);
```
	- !starvation 발생 가능.
	- 해결하려면? 일정시간까지 도착한 reader. 신호등처럼 끊어버리기. 
- Dining-Philosophers Problem
```c
semaphore chopstick[5];

do {
	P(chopstck[i];
	P(chopstick[(i + 1) % 5]);
	...
	eat();
	...
	V(chopstick[i]);
	V(chopstick[(i + 1) % 5]);
	...
	think();
	...
} while (1);
```
	- 앞 solution 문제점
		- deadlock 가능성
		- 모든 철학자가 동시에 배가 고파서 왼쪽 젓가락을 잡아버린 경우
	- 해결 방안
		- 4명의 철학자만이 테이블에 동시에 앉을 수 있도록 한다.
		- 젓가락을 두 개 모두 잡을 수 있을 때에만 젓가락을 잡을 수 있게 한다.
		- 비대칭
			- 짝수(홀수)철학자는 왼쪽(오른쪽) 젓가락부터 잡도록 한다.


> 2022.8.13(화)
```
문맥교환 : CPU 
문맥교환과 문맥교환이 아닌 것
	- CPU - interrupt or systemcall - CPU : 문맥교환 아니다 (지극히 일부만 저장, 오버헤드 적음, cpu의 register값 같은거.) -> cache memory flush가 오버헤드가 가장 크다(context switch)
	- CPU process A - CPU process B
- 프로세스를 스케줄링하기 위한 큐
	- job queue
	- ready queue
	- device queue
스케줄러 :
	- 장기스케줄러(long-term, job scheduler)
		- 시작 프로세스 중 어떤 것들을 ready queue로 보낼지.
		- 프로세스에 memory를 주는 문제
		- degree of multiprogramming(메모리에 프로그램이 몇개 올라갔는지)을 제어
		- time sharing system에는 보통 장기스케줄러 없음(바로 ready)
	- 단기스케줄러
		- 어떤 프로세스를 다음번에 running 시킬지 결정
		- 프로세스에 CPU를 주는 문제
		- 충분히 빨라야 함
	- 중기스케줄러
		- 여유 공간 마련을 위해 프로세스를 통째로 메모리에서 디스크로 쫓아냄
		- 프로세스에서 memory를 뺏는 문제
- 프로세스의 상태
	- running
	- ready
	- blocked
	- suspended(stopped) - 중기스케줄러에 의해 쫓겨난, 이외에도 있음(외부적인 요인). 운영체제가 메모리가없어서 프로세스를 쫓아냄. 사람이 프로세스를 갑자기 정지(리눅스환경에서 CTRL + Z)
- blocked 와 suspended 둘다 CPU가 없음. process는 일을하고 있는 상태. ssuspended는 멈춰있는 상태.
```

> keyword
```c
- 알고리즘 1, 2, 3
- 과잉양보
- Mutual Exclusion
- Bounded Waiting(유한 대기)
- Synchronization Hardware
- semaphore
	- basic
	- block / wakeup implementation
	- which is better?
```
# [19강. 병행제어I 3]
- 동시접근의 해석. (CPU가 하나여도 옮겨쓰는 과정에서 발생하는 문제까지 포함)
### Initial Attempts to Solve Problem
```c
do {
	entry section
	critical section
	exit section
	remainder section
} while (1);
```
- 프로세스들은 수행의 동기화(synchronize)를 위해 몇몇 변수를 공유할 수 있다. 

### Algorithm 1
```c
int turn;
(initially) turn = 0;
do {
	while (turn != 0) // my turn
	critical section
	turn 1;           // now it's your turn
	remainder section
} while (1)
```
- Satisfies mutual exclusion, but not progress
- 즉, 과잉양보: 반드시 한번씩 교대로 들어가야만 함(swap-turn) 그가 turn을 내 값으로 바꿔줘야만 내가 들어갈 수 있음. 특정 프로세스가 더 빈번히 critical section을 들어가야 한다면?

### Mutual Exclusion(상호 배제)
- 프로세스가 critical section 부분을 수행 중이면 다른 모든 프로세스들은 그들의 critical section에 들어가면 안 된다.
 
### Progress
- 아무도 critical section에 있지 않은 상태에서 critical section에 들어가고자 하는 프로세스가 있으면 critical section에 들어가게 해주어야 한다.
 
### Bounded Waiting(유한대기, starvation 배제)
- 프로세스가 critical section에 들어가려고 요청한 후부터 그 요청이 허용될 때까지 다른 프로세스들이 critical section에 들어가는 횟수에 한계가 있어야 한다.
 
### 가정
- 모든 프로세스의 수행 속도는 0보다 크다.
- 프로세스들 간의 상대적인 수행 속도는 가정하지 않는다.

### Algorithm 2
- Synchronization variables
	- boolean flag[2]
	- (initially) flag[모두] = false;
	- p1 ready to enter its critical section (if flag[i] == true)
```c
do {
	flag[i] = true; // pretend i am in
	while (flag[j]); // is he also In? then wait 
	critical section
	flag[i] = false;
	remainder section;
} while (1);
- Satisfies mutual exclusion, but not progress requirement
- 둘 다 2행까지 수행 후 끊임 없이 양보하는 상황 발생 가능.
```

### Algorithm 3(Peterson's algorithm)
```c
do {
	flag[i] = true;
	turn = j;
	while (flag[i] && turn == j)
		;
	critical section
	flag[i] = false;
	remainder section
} while (1);
```
- 간단해 보이지만 굉장히 심오한 코드다. flag[i]와 turn = j순서만 바껴도 제대로 동작하지 않는다. 경우를 따져보면 알 수 있다?
- Meets all requirement; solves the critical section problem for two processes.
- Busy waiting! (계속 CPU와 memory를 쓰면서 wait) 스핀락.

### Synchronization Hardware
- 하드웨어적으로 Test & modify를 atomic하게 수행할 수 있도록 지원하는 경우 앞의 문제는 간단히 해결 
```c
Synchronization variables:
	boolean lock = false;

Process P
	do {
		while (Test_and_Set(lock)); // 변수의 값을 READ하고 변수를 TRUE.
		critical section
		lock = false;
		remainder section
	}
```	

### Semaphores
- 앞의 방식들을 추상화 시킴
- Semaphore S (추상자료형 ABT)
	- integer variable
	- 아래의 두 가지 atomic 연산에 의해서만 접근 가능
		- P(S) : while (s < 0) do no-op; // P연산은 자원을 획득하는 과정
		  		s--;
- If positive, decrement & enter. Otherwise, wait until positive(busy-wait)
		- V(S)               //V연산은 자원을 반납하는 과정
				S++;
```c
semaphore mutex; // initially 1
do {
	P(mutex);	// if positive, dec & enter, Otherwise, wait
	critical section
	V(mutex);	// Increment semaphore
	remainder section
} while (1);
```

### Block / Wakeup Implementation
```
typedef struct
{
	int value; // semaphore
	sturct process *L;	// process wait queue
} semaphore;
```
- block과 wakeup을 다음과 같이 가정
	- block : 커널은 block을 호출한 프로세스를 suspend시킴. 이 프로세스의 PCB를 semaphore에 대한 wait queue에 넣음.
	- wakeup(P) : block된 프로세스 P를 wakeup시킴. 이 프로세스의 PCB를 ready queue로 옮김
- ex) L->PCB->PCB->PCB
- busy-waiting(=spin lock)은 효율적이지 못함(previous)
- Block & wakeup(=sleep lock)방식의 구현.
```
P(s)
S.value--; //0이되어있는 경우도 뺀다. 음수(여분이 없구나. process에 연결하고 잠들게 하는 거)
if (S.value < 0)
{
	add this process to S.L;
	block();
}

V(S)
S.value++; // 양수가 된다는 보장은 없다. 누군가가 세마포어를 기다리며 잠들어 있다. 
if (S.value <= 0)
{
	remove a process P from S.L;
	wakeup(P);
}
```
- 왜이렇게 했을까? 고민을 해보며 무릎을 탁

### which is better?
- Busy-wait vs block/wake up
- Block/wakeup overhead v.s Critical section 길이
	- Critical section의 길이가 긴 경우 Block/Wakeup이 적당
	- Critical section의 길이가 매우 짧은 경우 Block/Wakeup 오버헤드가 bust-wait 오버헤드보다 더 커질 수 있음 (-> 강의에서, 길이가 기냐 짧냐가 아니라 경합이 더욱 빈번하게 일어나냐 아니냐로 보는게 타당하다고 함. 크리티컬 섹션이 짧다 = 경쟁이 치열하지 않다로 이해)
	- 일반적으로는 Block/wakeup 방식이 더 좋음.

### Two Types of Semaphores
- Counting semaphore
	- 도메인이 0이상인 임의의 정수 값
	- 주로 resource counting에 사용
- Binary semaphore(=mutex)
	- 0 또는 1 값만 가질 수 있는 semaphore
	- 주로 mutual exclusion(lock/unlock)에 사용

### monitor

> keyword
```
```

# [18강. 병행제어I 2]
- 데이터의 접근: Race condition
- 설령 CPU가 하나 있더라도(시스템 콜 중 다른 프로세스에게 넘어가고, 그 프로세스가 시스템 콜에서 공유하는 데이터 값을 변경시킨 후 운영체제가 처음 프로세스에 데이타에 대한 시스템 콜을 할 때, 문제가 생김)

### OS에서 race condition은 언제 발생하는가? // 프로세스들간에는 주소공간을 침범하지 않지만, 커널은 공유데이타로 볼 수 있음.
1. Kernel 수행 중 `인터럽트` 발생 시
2. Process가 `System call`을 하여 kernel mode로 수행 중인데 context switch가 일어나는 경우
3. Multiprocessor에서 shared memory 내의 kernel data

### 1/3 If you preempt CPU while in kernel mode...
1. system call read()
2. Time quantum expires & ..
- 커널 모드에서 수행 중일 때는 CPU를 preempt하지 않음.
- 커널 모드에서 사용자 모드로 돌아올 때 preempt

### 2/3 interrupt

### 3/3 multiprocessor
- CPU가 여러개 있어도 운영체제가 실행될 때 문제가 되는 것이다.
- 어떤 CPU가 마지막으로 count를 store했는가? -> race condition. 
	- interrupt enable/disable로 해결되지 않음
1. 한번에 하나의 CPU만이 커널에 들어갈 수 있게 하는 방법
2. 커널 내부에 있는 각 공유 데이터에 접근할 때마다 그 데이터에 대한 lock/ unlock기법 적용.

### Process Synchronization 문제
- 공유데이터(shared data)의 동시 접근(concurrent access)은 데이터의 ㄷ불일치 문제(inconsistency)를 발생시킬 수 있다.
- 일관성(consistency)유지를 위해서는 협력 프로세스(cooperating process)간의 실행 순서(orderly execution)을 정해주는 메커니즘 필요

- Race condition
	- 여러 프로세스들이 동시에 공유 데이터를 접근하는 상황
	- 데이터의 최종 연산 결과는 마지막에 그 데이터를 다룬 프로세스에 따라 달라짐.
- Race condition을 막기 위해서는 concurrent process는 동기화(synchronize)되어야 한다.

### The critical-Section Problem
- n개의 프로세스가 공유 데이터를 동시에 사용하기를 원하는 경우
- 각 프로세스의 code segment에는 공유 데티러를 접근하는 코드인 critical section이 존재
- problem
	- 하나의 프로세스가 critical section에 있을 때 다른 모든 프로세스는 critical section에 들어갈 수 없어야 한다.

> Keyword
```
```

# [17강. 병행제어I 1]

### Multiple-Processor Scheduling (CPU가 여러개 있는 환경)
- CPU가 여러 개인 경우 스케줄링은 더욱 복잡해짐
- Homogeneous processor인 경우 (역량이 똑같은 CPU)
	- Queue에 한줄로 세워서 각 프로세서가 알아서 꺼내가게 할 수 있다.
	- 반드시 특정 프로세서에서 수행되어야 하는 프로세스가 있는 경우에는 문제가 더 복잡해짐
- Load sharing(Load balancing)
	- 일부 프로세서에 job이 몰리지 않도록 부하를 적절히 공유하는 메커니즘이 필요
	- 별개의 큐를 두는 방법 vs 공동 큐를 사용하는 방법
- Symmetric Multiprocessing(SMP) // CPU들이 대등
	- 각 프로세서가 각자 알아서 스케쥴링 결정
- Asymmetric multiprocessing // 대장 CPU
	- 하나의 프로세서가 시스템 데이터의 접근과 공유를 책임지고 나머지 프로세서는 거기에 따름

### Real-Time Scheduling // Deadline을 지키는 것이 중요하다.
- Hard real-time systems // 미리 스케쥴링을 해놓고 그거대로 따르게(offline)
	- Hard real-time task는 정해진 시간 안에 반드시 끝내도록 스케줄링해야 함.
- Soft real-time computing
	- Soft real-time task는 일반 프로세스에 비해 높은 priority를 갖도록 해야 함.

### Thread Scheduling // 운영체제가 직접 스케줄링을 하냐? 존재를 모른다면 그냥 프로세스에게 CPU를 줄 뿐.
- Local Scheduling
	- User level thread의 경우 사용자 수준의 thread library에 의해 어떤 thread를 스케줄할지 결정
- Global Scheduling
	- Kernel level thread의 경우 일반 프로세스와 마찬 가지로 커널의 단기 스케줄러가 어떤 thread를 스케줄할지 결정 // 운영체제가 별도의 스레드가 있다는 사실을 안다.

### Algorithm Evaluation
- Queuing models
	- 확률 분포로 주어지는 arrival rate와 service rate 등을 통해 각종 performance index 값을 계산
- Implementation(구현) & Measurement(성능 측정)
	- 실제 시스템에 알고리즘을 구현하여 실제 작업(workload)에 대해서 성능을 측정 비교
- Simulation(모의 실험)
	- 알고리즘을 모의 프로그램으로 작성 후 trace를 입력으로 하여 결과 비교

> keyword
```c
```
## [15, 16강. CPU 스케쥴링 3, 4]
### FCFS(First-Come First-Served)
- Convoy effect (short process behind long process)
 
### SJF(Shortest Job First)
- 각 프로세스의 다음번 CPU burst time을 가지고 스케줄링에 활용
- CPU burst time이 가장 짧은 프로세스를 제일 먼저 스케줄
	- Nonpreemptive
		- 일단 CPU를 잡으면 이번 CPU burst가 완료될 때까지 CPU를 선점(preemption) 당하지 않음
	- Preemptive (더 짧음)
		- 현재 수행중인 프로세스의 남은 burst time보다 더 짧은 CPU burst time을 가지는 새로운 프로세스가 도착하면 CPU를 빼앗김
		- 이 방법을 Shortest-Remaining-Time-First(SRTF)이라고도 부른다.
- SJF is optimal
	- 주어진 프로세스들에 대해 minimum average waiting time을 보장.
- 문제
	- Starvation
	- 다음 CPU Burst Time 예측
		- 추정만이 가능하다.
		- 과거의 CPU burst time을 이용해서 추정
			- 식의 의미: n+1번째 CPU burst time = 바로 직전의 CPU burst값하고 n번째 예측했던 값을 더한다. (가중치를 해준다)
			- 식을 풀게되면 후속 term은 선행 term보다 적은 가중치 값을 지닌다. (exponential average)

### SRTF(Shortest-Remaining-Time-First)
 
### Priority Scheduling
- A priority number(integer) is associated with each process.
- highest priority를 가진 프로세스에게 CPU할당(smallest integer = highest priority)
	- Preemptive
	- nonpreempive
- SJF는 일종의 priority scheduling이다.
	- priority = predicted next CPU burst time
- Problem
	- starvation : low priority processes may never execute.
- Solution
	- Aging : as time progresses increase the priority of the process.

### RR (Round Robin)
- 각 프로세스는 동일한 크기의 할당 시간(time quantum)을 가짐 (일반적으로 10~100 millisecond)
- 할당 시간이 지나면 프로세스는 선점(preemptive)당하고 ready queue의 제일 뒤에 가서 다시 줄을 선다.
- n개의 프로세스가 ready queue에 있고 할당 시간이 q time unit인 경우 각 프로세스는 최대 q time unit 단위로 CPU 시간의 1/n을 얻는다.
	- 어떤 프로세스도 (n-1)q time unit이상 기다리지 않는다.
- Performances
	- q large = FCFS
	- q small = context switch 오버헤드가 커진다.
- 일반적으로 SJF보다 average turnaround time이 길지만 Response time(응답시간)이 짧다.
- 비슷한 잡들이 있을 때 쓰면 비효율.
- 기다리는 시간이 본인의 CPU사용시간에 비례하므로 굉장히 공평하다.

### Multilevel Queue
- CPU는 하난데 여러줄
- Ready queue를 여러 개로 분할
	- foreground(interactive)
	- background(batch - no human interface)
- 각 큐는 독립적인 스케쥴링 알고리즘을 가짐
	- foreground - RR
	- background - FCFS
- 큐에 대한 스케줄링이 필요
	- Fixed priority scheduling
		- serve all from foreground then from background.
		- possibility of starvation
	- Time slice
		- 각 큐에 CPU tim을 적절한 비율로 할당
		- Eg. 80% to foreground in RR, 20% to background in FCFS
		 
### Multilevel Feedback Queue
- 프로세스가 다른 큐로 이동 가능
- 에이징(aging)을 이와 같은 방식으로 구현할 수 있다.
- Multilevel-feedback-queue scheduler를 정의하는 파라미터들
	- Queue의 수
	- 각 큐의 scheduling algorithm
	- process를 상위 큐로 보내는 기준
	- process를 하위 큐로 내쫓는 기준
	- 프로세스가 CPU 서비스를 받으려 할 때 들어갈 큐를 결정하는 기준

// 나는 blocked I/O의 경우 CPU제어권이 P0->운영체제->P0이고 이 과정중에는 컨텍스트 스위치가 실행되지도 않는다. 그래서 프로세스의 상태도 Running중으로 계속 보는 것이다. (CPU가 운영체제에 있더라도 프로세스는 실행중 상태에 있으니까) -> 넌 굉장히 중요한 I/O다. 완료되자마자 Ready queue에 들어가는 것이 아닌, 바로 실행되어야 한다.

// 나는 non_blocked I/O의 경우CPU제어권이 P0->운영체제, P1(운영체제는 I/O device queue에 던지고, P1에게 CPU를 준다) 컨텍스트 스위치가 일어났고, 프로세스의 상태 P0은 blocked이며, P1은 Running상태인 것이다.

## [14강. CPU 스케쥴링 2]
### CPU and I/O Bursts in Program Execution
- CPU burst (CPU를 가지고 기게어를 실행)
- I/O burst (I/O단계)
	 - CPU를 길게 쓰는 프로그램(CPU bound job)
	 - I/O를 많이 쓰는 프로그램(I/O bound job)
- cpu 스케쥴링을 할 때 I/O bound job에게 먼저 cpu를 주는 방법이 필요하겠다. (CPU스케쥴링이 필요하다)
	- interactive job에게 적절한 response 제공..

### 프로세스의 특성 분류
- I/O-bound process
	- CPU를 잡고 계산하는 시간보다 I/O에 많은 시간이 필요한 job
	- (many short CPU bursts)
- CPU-bound process
	- 계산 위주의 job
	- (few very long CPU bursts)

### CPU Scheduler & Dispatcher
- CPU Scheduler
	- Ready 상태의 프로세스 중에서 이번에 CPU를 줄 프로세스를 고른다.
- Dispatcher
	- CPU의 제어권을 CPU scheduler에 의해 선택된 프로세스에게 넘긴다.
	- 이 과정을 context switch(문맥 교환)이라고 한다.
- CPU 스케쥴링이 필요한 경우는 프로세스에게 다음과 같은 상태 변화가 있는 경우이다.
	- Running -> Blocked (ex: I/O) // I/O가 하는 친구가 자진적으로 CPU를 내려논다고?
	- Running -> Ready (ex: 할당시간 만료로 timer interrupt)
	- Blocked -> Ready (ex: I/O 완료 후 인터럽트) 
	- Terminate
- 1,4에서 스케쥴링은 non-preemptive(=강제로 빼앗지 않고 자진 반납)
- all other scheduling is preemptive(=강제로 빼앗음)

### Scheduling Criteria
- 1. CPU utilization(이용률)
	- keep the CPU as busy as possible
- 2. Throughput(처리량)
	- of processes that complete their execution per time unit
- 3. Turnaround time(소요시간, 반환시간)
	- amount of time to execute a particular process
- 4. Waiting time(대기시간)
	- amount of time a process has benn waiting in the ready queue
- 5. Response time(응답시간)
	- amount of time it takes from when a request was submitted until the first response is produced, not output
 
### 
> 2022.09.08(목)
```
- 프로그램 실행 (메모리 + disk) => virtual memory
- virtual memory
	- stack
	- data
	- code
	- heap
- 커널 memory
	- code : 커널 코드(인터럽트, 자원 처리)
	- data : PCB, CPU, MEM, DISK
	- stack : `process A의 커널 스택` `프로세스 B의 커널 스택`
- 시스템콜(user->kernel->user)	

- PCB
```
> keyword
```
fork()
exec()
wait()
exit()
프로세스간 협력
	- 통신(IPC)
		- message passing
		- shared memory
```
## [13강. CPU 스케쥴링 1]
- execlp 설명

### wait()시스템 콜
- sleep until child is done
- 프로세스 A가 wait() 시스템 콜을 호출하면
	- 커널은 child가 종료될 떄까지 프로세스 A를 sleep시킨다(block 상태)
	- child process가 종료되면 커널은 프로세스 A를 깨운다.(ready 상태)

### exit()시스템 콜
- free all the resources, notify parent
- 프로세스의 종료
	- 자발적 종료
		- 마지막 statement 수행 후 exit()시스템 콜을 통해
		- 프로그램에 명시적으로 적어주지 않아도 main 함수가 리턴되는 위치에 컴파일러가 넣어줌
	- 비자발적 종료
		- 부모 프로세스가 자식 프로세스를 강제 종료시킴
			- 자식 프로세스가 한계치를 넘어서는 자원 요청
			- 자식에게 할당된 테스크가 더 이상 필요하지 않음
		- 키보드로 kill, break  등을 친 경우 // 윈도우 x, ctrl c
		- 부모가 종료하는 경우 
			- 부모 프로세스가 종료하기 전에 자식들이 먼저 종료됨.

### 프로세스 간 협력
- 독립적 프로세스(Independent process)
	- 프로세스는 각자의 주소 공간을 가지고 수행되므로 원칙적으로 하나의 프로세스는 다른 프로세스의 수행에 영향을 미치지 못함
- 협력 프로세스(Cooperating process)
	- 프로세스 협력 메커니즘을 통해 하나의 프로세스가 다른 프로세스의 수행에 영향을 미칠 수 있음.
- 프로세스간 협력 메커니즘(IPC, interprocess communication)
	- 메시지를 전달하는 방법
		- message passing: 커널을 통해 메시지 전달
	- 주소 공간을 공유하는 방법
		- shared memory : 서로 다른 프로세스 간에도 일부 주소 공간을 공유하게 하는 shared memory 메커니즘이 있음
		cf. thread : thread는 사실상 하나의 프로세스이므로 프로세스 간 협력으로 보기는 어렵지만 동일한 process를 구성하는 thread간에는 주소 공간을 공유하므로 협력이 가능.

### Message Passing
- 프로세스 사이에 공유 변수(shared variable)를 일체 사용하지 않고 통신하는 시스템.
	- Direct communication
		- 통신하려는 프로세스의 이름을 명시적으로 표시
	- Indirect communication
		- mailbox(또는 port)를 통해 메시지를 간접 전달
(사이에 커널이 껴 있다)
		
### Shared Memory
- 커널에게 이 부분을 공유하겠습니다. 요청
- 이 프로세스가 서로 신뢰할 수 있는가? (전제)

> 9.26(수)
```
- synchornous I/O
- asynchronus I/O
- DMA
- 저장장치 계층 구조(캐싱)

동기, 비동기
blocked, non-blocked

동기/blocked, non-blokced
```

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
- create a child(copy)
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
- overlay new image
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
	- 구현방법 1 (blocked I/O)
		- I/O가 끝날 때까지 CPU 낭비시킴
		- 매시점 하나의 I/O만 일어날 수 있음
	- 구현방법 2 (non-blocked I/O)
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

