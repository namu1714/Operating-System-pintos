# 2020 OS Pintos Project

## What is Pintos?
simple operating system framework for the 80x86 architecture.   
It supports basic functionalities and provides us some skeletons.
- Kernel threads
- Loading and running user programs
- Simple file system
- Simple memory management

## *Project 0: Data Structures in Pintos Kernel
핀토스 커널의 List, Hash table, Bitmap 자료구조를 테스트하는 interactive program 구현

## *Project 1: User Program
- **Argument Passing** - 입력받은 argument들을 파싱해 80x86 calling convention에 따라 유저 스택에 순차적으로 쌓기   

- **User Memory Access** - user pointer가 valid한지 체크하여 커널 등 허용되지 않은 영역을 참조할 경우 프로그램을 종료
- **System Call Handler** - System call handler 함수를 구현
- **System Call Implementation** - halt, wait, exec, exit, write, read 구현
- **Additional Implementation** - fibonacci(), max_of_four_int() 시스템 콜을 추가로 구현


## *Project 2: File System
- **File Descriptor**   

- **System Calls** - create, remove, open, filesize, read, write, seek, tell과 같은 file system 관련 시스템 콜들을 구현
- **Synchronization in Filesystem** - lock, semaphore 사용

## *Project 3: Threads
- **Alarm Clock** -
핀토스에서 thread가 잠들었을 경우 사용하던 기존의 busy-waiting 기법을 수정하여, 시간이 다 되면 BLOCKED 상태의 스레드를 깨우는 alarm clock을 구현

- **Priority Scheduling** -
round-robin scheduler에 priority를 반영하고, 일정 시간마다 ready list에 있는 thread들의 priority를 높여주는 aging 시스템을 구현해 starvation 현상을 예방	
- **Advanced Scheduler** -
Multi-Level Feedback Queue(MLFQ)를 사용하는 BSD scheduler를 개발
