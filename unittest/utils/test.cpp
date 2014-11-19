//
//  test.cpp
//  console
//
//  Created by huanghaifeng on 14-10-23.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <boost/asio.hpp>
#include "timer.h"
#include "time_utils.h"

void timeout(int tag, frog::utils::timer* t)
{
    printf("tag:%d, timeout, %s\n",tag,frog::utils::get_string_time((int)time(NULL)).c_str());
    if (tag==1) {
        t->add_task(1, 3, timeout);
        //t->del_task(3);
    }
    if (tag==3) {
        t->add_task(3, 5, timeout);
    }
}

int main()
{
    boost::asio::io_service ioserv;
    frog::utils::timer t(ioserv);
    printf("tag:%d, %s\n",1,frog::utils::get_string_time((int)time(NULL)).c_str());
    t.add_task(1, 3, timeout);
    t.add_task(2, 4, timeout);
    t.add_task(3, 5, timeout);
    ioserv.run();
    return 0;
}

#if 0
#include "dbthread.h"
#include <iostream>
#include <stdio.h>

void dbthread_query_handler(int tag, frog::db::dataset* ds)
{
    if (!ds)
    {
        printf("sql(%d) query failed",tag);
        return;
    }
    
    switch (tag)
    {
        case 1:
        {
            for (unsigned long i = 0; i < ds->row_count(); i++)
            {
                int id = ds->get_field_int32(i,"id");
                std::string name = ds->get_field_string(i,"name");
                printf("row%ld : id(%d),name(%s)\n",i,id,name.c_str());
            }
            break;
        }
            
        default:
        {
            printf("sql(%d) query success but ignored",tag);
            break;
        }
    }
}

void dbthread_execute_handler(int tag, bool ret)
{
    printf("sql(%d) execute %s\n",tag,(ret?"success":"failed"));
}

void dbthread_error_handler(int tag, std::string error)
{
    printf("sql(%d) error: %s\n",tag,error.c_str());
}

int main()
{
    frog::db::conn_args_t connargs = {"127.0.0.1","3306","root","123123","test"};
    frog::utils::dbthread t(frog::db::e_mysql, connargs);
    t.set_error_callback(dbthread_error_handler);
    t.set_execute_callback(dbthread_execute_handler);
    t.set_query_callback(dbthread_query_handler);
    t.init();
    
    int tag = 1;
    t.query(tag, "SELECT * FROM test.t2;");
    for (tag=2; tag<10; ++tag)
    {
        //t.execute(tag, "INSERT INTO test.t2(`name`) VALUES('wong');");
        //printf("push sql(%d)\n",tag);
    }
    
    t.join();
    return 0;
}
#endif 

#if 0
#include <thread>
#include <iostream>

#define INFINITE_QUEUE 0

#if INFINITE_QUEUE
#include "srsw_queue.hpp"
frog::utils::srsw_queue<int> q;
frog::utils::srsw_queue<char> signal1;
frog::utils::srsw_queue<char> signal2;
#else
#include "srsw_circular_queue.hpp"
frog::utils::srsw_circular_queue<int,1024> q;
frog::utils::srsw_circular_queue<char,1> signal1;
frog::utils::srsw_circular_queue<char,1> signal2;
#endif

void thread1()
{
    int i = 0;
    char c(0);
    while (true) {
        ++i;
        q.push(i);
        if (signal1.pop(c) && c=='q') {
			signal2.push(c);
            break;
        }
        std::chrono::milliseconds dura( 100 );
        std::this_thread::sleep_for( dura );
    }
}

void thread2()
{
    int i(0);
	char c(0);
    while (true) {
        if (q.pop(i))
		{ std::cout << i << " " << std::flush; } 
		if (signal2.pop(c) && c=='q') 
		{ break; }
    }
}

int main()
{
#if INFINITE_QUEUE
    q.init();
    signal1.init();
	signal2.init();
#endif

    std::thread t1(&thread1);
    std::thread t2(&thread2);
    char ch;
    while (true) {
        std::cin >> ch;
        if (ch=='q') { break; }
    }
    signal1.push(ch);

    t1.join();
    std::cout << "thread1 done" << std::endl;
    t2.join();
    std::cout << "thread2 done" << std::endl;

#if INFINITE_QUEUE
    q.fini();
    signal1.fini();
	signal2.fini();
#endif
	
#ifdef _WIN32
	system("pause");
#endif

    return 0;
}
#endif

