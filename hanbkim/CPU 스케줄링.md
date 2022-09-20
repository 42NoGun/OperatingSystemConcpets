# CPU 스케줄링

### CPU-burst Time의 분포

![스크린샷 2022-09-16 오전 2.06.56.png](CPU%20%E1%84%89%E1%85%B3%E1%84%8F%E1%85%A6%E1%84%8C%E1%85%AE%E1%86%AF%E1%84%85%E1%85%B5%E1%86%BC%20298cfe7ce57544f98769fbf4e11111a9/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2022-09-16_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%258C%25E1%2585%25A5%25E1%2586%25AB_2.06.56.png)

### 프로세스의 특성 분류

- I/O-bound process
→ CPU를 잡고 계산하는 시간보다 I/O처리에 많은 시간이 필요한 job (many short CPU bursts)
- CPU-bound process
→ CPU 연산 위주의 job (few very long CPU bursts)

### CPU Scheduler & Dispatcher

- CPU scheuler
→ Ready 상태의 프로세스 중에서 CPU를 넘길 프로세스를 결정한다
- Dispatcher
→ CPU의 제어권을 스케줄러에 의해 선택된 프로세스에게 **부여한다.**
== 이 과정을 문맥 교환(context switch) 라고 한다.

스케줄링이 필요한 경우는..

1. Running에서 blocked로 넘어갈 경우
2. Running에서 Ready로 넘어갈 경우
3. Blocked에서 Ready로..
4. Treminated

1, 4에서 스케줄링은 nonpreemptive (프로세스가 자진 반납함)
나머지 스케줄링들은 preemptive (운영체제가 강제로 뻇음)

### Scheduling Criteria (Performance Index = Performance, 성능 척도)

- CPU utilzation 이용률
→ CPU를 얼마나 가동했나
- Throughput 처리량
→ CPU가 일을 얼마나 끝냈나
- Turnarount time 소요 시간, 반환 시간
→ CPU를 사용한 시간, CPU를 기다린 시간의 합이다.
- Waiting time
→ CPU를 기다린 시간의 합이다.
- Response time
→ 프로세스가 처음 시작해서 CPU를 얻은시간 X, IO가 끝나고 cpu burst에 들어와서 (CPU를 쓰러 들어와서) 처음 CPU를 얻기까지의 시간

### CPU 스케줄링 알고리즘의 종류..

- FCFS (First-Come First-Served)
들어온 순서대로..
- SJF (Shortest-Job-First)
Cpu burst time이 짧은 순서대로..
가장 짧은 Cpu burst time 평균시간을 가진다.
SJF의 구현에 Nonpreemptive preemptive(Shortest-Remaining-Time-First) 두 방식이 있다.
- Priority Scheduling
우선 순위 숫자 integer를 하나둬서 높은 숫자를 가진 프로세스에게 cpu를 할당한다.
Nonpreemptive preemptive 두 방식 다 구현 가능하다.
SJF는 일종의 priority scheduling이다..
Starvation : 영원히 실행되지 않을 수 있다.
해결법 → Aging : 프로세스가 오래 기다렸으면 우선순위를 조금씩 높인다.
- RR (Round Robin)
각 프로세스는 동일한 크기의 할당 시간(time quantum)을 가진다.
할당 시간이 지나면 프로세스는 preempted 당하고 레디큐에 다시 줄을 선다.
n개의 프로세스가 레디큐에 있고 할당 시간이 q time unit인 경우 각 프로세스는 최대 q time unit 단위로 CPU 시간의 1/n을 얻는다.
⇒ 어떤 프로세스도 (n-1) q time unit 이상 기다리지 않는다.
q 시간이 커지면 FCFS와 다를게 없고,
q 시간이 너무 context switch때문에 적다면 오버헤드가 커진다..
일반적으로 SJF보다 average turnaround time이 길지만 response time은 짧다.
long job과 short job가 섞여있기 때문에 rr을 쓴다.. 모두 같은 시간이라면 이 알고리즘을 굳이 쓸 필요가 없다.

하나의 cpu로 여러개의 큐를 쓰는 방법도 있다.

- Multilevel Queue
각 큐는 독립적인 스케줄링 알고리즘을 가진다.
    - foreground (interactive) → RR
    - background (batch - no human interaction) → FCFS
    
    큐에 대한 스케줄링도 필요하다..
    
    - Fixed priority scheduling
        - serve all from foreground then from background.
        - Possibility of starvation
    - Time slice
        - 각 큐에 cpu time을 적절한 비율로 할당한다.
        80%를 RR(foreground), 20%를 FCFS(background)
- Multilevel Feedback Queue
    - 프로세스가 다른 큐로 이동이 가능하다. (신분 상승, 하락 가능)
    - aging을 이와 같은 방식으로 구현 가능하다
    - 큐의 개수, 각 큐의 스케줄링 알고리즘, 프로세스를 어느 큐로 보낼지, 프로세스의 우선순위를 결정하는 기준 등을 정해야 한다.
        
        ![스크린샷 2022-09-20 오전 1.04.41.png](CPU%20%E1%84%89%E1%85%B3%E1%84%8F%E1%85%A6%E1%84%8C%E1%85%AE%E1%86%AF%E1%84%85%E1%85%B5%E1%86%BC%20298cfe7ce57544f98769fbf4e11111a9/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2022-09-20_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%258C%25E1%2585%25A5%25E1%2586%25AB_1.04.41.png)
        

### 다음 CPU Burst Time의 예측

어떻게 알 수 있을까..?  (input data, branch,  user … )

정확히 알 수는 없고, 추정만 가능하다. 과거의 CPU burst time을 이용해서..

![스크린샷 2022-09-19 오전 1.43.08.png](CPU%20%E1%84%89%E1%85%B3%E1%84%8F%E1%85%A6%E1%84%8C%E1%85%AE%E1%86%AF%E1%84%85%E1%85%B5%E1%86%BC%20298cfe7ce57544f98769fbf4e11111a9/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2022-09-19_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%258C%25E1%2585%25A5%25E1%2586%25AB_1.43.08.png)

![스크린샷 2022-09-19 오전 1.47.37.png](CPU%20%E1%84%89%E1%85%B3%E1%84%8F%E1%85%A6%E1%84%8C%E1%85%AE%E1%86%AF%E1%84%85%E1%85%B5%E1%86%BC%20298cfe7ce57544f98769fbf4e11111a9/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2022-09-19_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%258C%25E1%2585%25A5%25E1%2586%25AB_1.47.37.png)

### Multiple-Processor Scheduling

cpu가 여러개인 경우 스케줄링은 더욱 복잡해진다..

- Homogeneous processor인 경우
    - 큐에 한 줄로 세워서 프로세서가 알아서 꺼내가게 할 수 있다.
    - 반드시 특정 프로세서에서 수행되어야 하는 프로세스가 있는 경우에는 문제가 더 복잡해진다.
- Load sharing
    - 일부 프로세서에 job이 몰리지 않도록 부하를 적절히 공유하는 메커니즘이 필요하다
    - 별개의 큐를 두거나, 공동 큐를 사용하거나

- Symmetric Multiprocessing (SMP)
    - 각 프로세서(CPU)가 각자 알아서 스케줄링 결정
- Asymmetric Multiprocessing
    - 하나의 프로세서(CPU)가 시스템 데이터의 접근과 공유를 책임지고 나머지 프로세서는 거기에 따른다.
    

### Real-Time Scheduling

- Hard real-time systems
정해진 시간에 반드시 끝내도록 스케줄링 해야함..
- Soft real-time computing
일반 프로세스에 비해 높은 priority를 갖도록 해야한다.. 데드라인을 반드시 보장해주지 못하지만 cpu를 먼저 얻을수 있게 돕는다.

### Thread scheduling

- Local Scheduling
사용자 수준의 thread library에 의해 어떤 thread를 스케줄할지 결정한다. 운영체제는 갖고있는지 모른다.. 프로세스 스스로가 어떤 쓰레드를 스케줄할지 판단한다.
- Global Scheduling
커널의 단기 스케줄러가 thread를 스케줄할지 결정한다.

### Algorithm Evaluation

- Queueing models
확률 분포로 주어지는 arrival rate와 service rate를 통해 각종 preformance index 값을 계산한다.
- Implementation & measurement
실제 시스템에 알고리즘을 구현하여 실제 작업에 대해서 성능을 측정 후 비교한다.
실제 구현은 굉장히 쉽지 않다..
- Simulation 
알고리즘을 모의 프로그램으로 작성 후 trace를 입력으로 하여 결과를 비교한다.