### 信号
&emsp; 信号是一种异步通信机制，信号的来源可以来自硬件，也可以来自软件。

### 可靠信号和不可靠信号
&emsp; 把一些从早期继承发展过来的信号叫不可靠信号。它们主要有两个问题：1.每次信号处理后，就对信号的响应设为默认动作，用户如果不希望这样操作，必须重新signal()一次；2.信号可能丢失。  
&emsp; 把对原有信号机制进行扩充，信号值大于SIGRTMIN小于SIGRTMAX的信号称为可靠信号，这些信号克服了信号可能丢失的问题。

### 实时信号和非实时信号
&emsp; 非实时信号都不支持排队，都是不可靠信号；实时信号都支持排队，都是可靠信号。

### 进程对信号的响应
&emsp; 进程可以通过三种方式来响应一个信号。1.忽略这个信号，但是SIGKILL和SIGSTOP无法忽略；2.捕捉这个信号，即定义这个信号的信号处理函数，当信号发生时，执行信号处理函数；3.进行缺省操作，Linux对于每个信号都规定了默认的缺省操作，注意，进程对实时信号的缺省操作都是进程终止。

### 信号的发送
&emsp; 发送信号的主要函数有：kill、raise、sigqueue、alarm、setitimer、abort。
1. kill，原型为int kill(pid_t pid, int signo), signo是信号值。
2. raise, 原型为int raise(int signo), 向进程本身发送信号。
3. sigqueue，原型为int sigqueue(pid_t pid, int sig, const union sigval val),前两个参数为pid和发送的信号值，第三个指定了信号传递的参数。在调用sigqueue会将传递的参数拷贝到信号处理函数。
4. alarm，原型为unsigned int alarm(uisigned int seconds), 专门为SIGALRM信号设定，执行的seconds后向进程本身发送SIGALRM信号。进程调用alarm，任何之前调用的alarm都将无效。而返回值，如果在调用alarm前，进程已经调用了闹钟，则返回上一个闹钟的剩余时间，否则返回0。
5. setitimer, 原型为int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue),第二个参数是itimerval的一个实例，第三个参数可以忽略。第一参数可以指定时钟的类型：
```
1. ITIMER_REAL: 设定绝对时间，经过指定的时间后，内核发送SIGALRM信号给本进程。
2. ITIMER_VIRTUAL: 设定程序执行时间，经过指定的时间后，发送SIGALRM信号给本进程。
3. ITIMER_PROF: 设定进程执行以及内核因本进程而消耗的时间和，经指定的时间后，内核将发送ITIMER_VIRTUAL信号给本进程
```
6. abort, 原型为void abort(void), 向进程发送SIGABORT信号，默认情况下进程会退出。

### 信号的安装
&emsp; 如果进程要处理一个信号，那么就要在进程中安装这个信号，安装信号主要确定信号值和该信号对信号的动作之间的映射。信号的安装主要有两个函数signal()、sigaction():
1. signal, 原型为 void signal(int signnum, void (*handler)(int))(int), 为signnum安装handler操作。
2. sigaction，原型为int sigaction( int signnum, const struct sigaction *act, struct sigaction *oldact)。用于定义进程就收特定信号后的行为，第二信号最为重要，其中包含了对特定信号的处理，信号传递的信息，信号处理过程中应屏蔽的函数等等。
```
struct sigaction
{
	union
  	{
		__sighandler_t _sa_handler;
		void (* _sa_sigaction)( int, struct siginfo *, void *);
	}_u;
	
  	sigset_t sa_mask;
	unsigned long sa_flags;
}

1. 在联合结构体_u中，由_sa_handler以及*_sa_sigaction指定信号关联函数，即用户指定的信号处理函数。由_sa_handler指定的处理函数只能由一个参数，即信
号值。而由_sa_sigaction指定的处理函数可以带三个参数，它指定一个3个参数的信号处理函数，第一个参数为信号值，第三个参数没有用，第二个参数的是指向
siginfo_t结构的指针，结构体中包含信号携带的数据值。

siginfo_t {
	int si_signo; 		//信号值
	int si_errno; 		//error值
	int si_code;  		//信号产生的原因
	pid_t si_pid; 		//发送信号的进程ID,对于kill(2),实时信号以及SIGCHLD有意义
	uid_t si_uid;		//发送信号进程的真实用户ID，对于kill(2)，实时信号以及SIGCHLD有意义
	int si_status;		//退出状态，对SIGCHLD有意义
	clock_t si_utime;	//用户消耗的时间，对SIGCHLD有意义
	clock_t si_stime;	//内核消耗的时间，对SIGCHLD有意义
	sigval_t si_value;	//信号值，对于所有实时信号有意义，是一个联合结构体
	
	void* si_addr;		//触发fault的内存地址，对于SIGILL,SIGFPE,SIGSEGV,SIGBUS 信号有意义
	int si_band;		//对于SIGPOLL有意义
	int si_fd;		//对于SIGPOLL有意义
}

si_value是一个联合结构体，结构如下：
union sigval{
	int sival_int;
	void *sival_ptr;
}

前面讨论的sigqueue发送信号时，sigqueue的第三个参数就是sigval联合结构体，当调用sigqueue时，该数据结构中的数据接拷贝到信号处理函数的的第二
个参数中。

2. sa_mask指定在信号处理过程中，哪些信号应当被阻塞。缺省情况下当前信号本身因该被阻塞，防止信号的嵌套发送，除非指定了SA_NIDEFER或者SA_NOMASK
标志位。

3.sa_flags中包含了许多的标志位，包括SA_NODEFER以及SA_NOMASK标志位。另一个比较重要的标志位是SA_SIGINFO，当设定了该标志位时，表示该信号附带
的参数可以被传递到信号处理函数中，因此，在sigaction结构体中的sa_sigaction指定处理函数，而不因该为sa_handler指定信号处理函数，否则，这个设置
将变得含无意义。
```

### 信号集和信号集操作函数
&emsp; 信号集用来描述信号的集合，linux所支持的所有信号可以全部或者部分地出现在信号集中，主要与信号阻塞相关函数配合使用。
```
typedef struct
{
	unsigned long sig[_NSIG_WORDS];
} sigset_t;

相关的函数有sigemptyset、sigfillset等等。
```

### 信号阻塞和信号未决
&emsp; 每个进程都有一个用来描述哪些信号传递到进程时将被阻塞的信号集，该信号集中的所有信号在递送到进程后将被阻塞。下面是三个相关的函数：
```
int sigpromask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
int sigsuspend(const sigset_t *mask);

sigpromask()函数能根据参数how来实现对信号的操作，如下
1. SIG_BLOCK , 在进程阻塞信号集合中添加set中的信号
2. SIG_UNBLOCK， 在进程的阻塞信号集合中去除set中的信号
3. SIG_SETMASK, 更新进程阻塞信号集为set

sigpending(sigset_t *set)可以返回已经到达该进程却被阻塞的信号

sigsuspend( const sigset *mask) 用于在接受某个信号前，临时用mask替换进程的掩码，并暂停进程的执行，直到接受到信号为止。
```

