#include "dboperator.h"
#include <stdio.h>

#if 0
using namespace frog::db;

/*

CREATE TABLE `t1` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(200) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=latin1

CREATE TABLE `t2` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(200) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8

*/
void print_dataset(dataset* ds)
{
	std::cout << "----------" << std::endl;
	if (!ds) { std::cout << "query db failed" << std::endl; return; }
	for (unsigned long i = 0; i < ds->row_count(); i++)
	{
		int id = ds->get_field_int32(i,"id");
		std::string name = ds->get_field_string(i,"name");
		printf("row%ld : id(%d),name(%s)\n",i,id,name.c_str());
	}
}

int main()
{
    // postgresql
    conn_args_t connargs = {"127.0.0.1","5432","postgres","","test"};
    //const char* selectallsql1 = "SELECT * FROM t1;";
    const char* selectallsql2 = "SELECT * FROM t2;";
    dboperator oper(e_postgre);
    
    // mysql
	//conn_args_t connargs = {"127.0.0.1","3306","root","123123","test"};
    //conn_args_t connargs = {"192.168.100.170","3388","root","123123","test"};
	//const char* selectallsql1 = "SELECT * FROM test.t1;";//MyISAM
	//const char* selectallsql2 = "SELECT * FROM test.t2;";//InnoDB
	//dboperator oper(e_mysql);
    
	if (!oper.connect(connargs))
	{ std::cout << "connect db failed" << std::endl; }

#if 0
	if (!oper.execute("INSERT INTO t1(id,name) VALUES(5,'wong');"))
	{ std::cout << "insert db failed" << std::endl; }
	
	print_dataset(oper.query(selectallsql1));
    
	if (!oper.execute("UPDATE t1 SET name = 'hoi' WHERE id=5"))
	{ std::cout << "update db failed" << std::endl; }
    
    print_dataset(oper.query(selectallsql1));
#endif
    
#define TEST_TRANSACTION
#ifdef TEST_TRANSACTION
	print_dataset(oper.query(selectallsql2));

	// transaction
	if (!oper.start_transaction())
	{
		std::cout << "start_transaction failed" << std::endl;
		//goto FINALLY;
        return 0;
	}

	if (!oper.execute("UPDATE t2 SET name = 'hoi' WHERE id=5"))
	{
		std::cout << "update db failed" << std::endl;
		//goto FINALLY;
        return 0;
	}
	
	char c(0);
	std::cout << "Commit or Rollback" << std::endl;
	std::cin >> c;

	if (c=='c')
	{
		std::cout << "commiting" << std::endl;
		if (!oper.commit())
		{
			std::cout << "commit failed" << std::endl;
			goto FINALLY;
		}
	}

	if (c=='r')
	{
		std::cout << "rollbacking" << std::endl;
		if (!oper.rollback())
		{
			std::cout << "rollback failed" << std::endl;
			goto FINALLY;
		}
	}
	
	if (!oper.stop_transaction())
	{
		std::cout << "stop_transaction failed" << std::endl;
	}

	// after transaction
	if (!oper.execute("UPDATE t2 SET name = 'helloworld!!!' WHERE id=7"))
	{ std::cout << "update db failed" << std::endl; }

	print_dataset(oper.query(selectallsql2));
#endif

FINALLY:

#ifdef _WIN32
	system("pause");
#endif

	return 0;
}
#endif
