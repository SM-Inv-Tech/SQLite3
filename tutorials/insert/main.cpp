////////////////////////////////////////////////////////////////////////
/// \file      main.cpp
/// \copyright Copyright (c) 2022 标准模型投资有限公司.
///            All rights reserved.
///            Licensed under the MIT License.
/// \author    金小海
/// \date      2022年09月18日, Sun, 13:50
/// \version   1.0
/// \brief
#include <iostream>

#include "fmt/ranges.h"

#include "xhsqlite3.hpp"

struct Employee
{
    int id;
    std::string name;
    int age;
    std::string address;
    double salary;
};

struct DetailedEmployee : public Employee
{
    std::string birthDate;
};

using namespace xhsqlite3;

int main(int argc, char **argv)
{
    auto storage = make_storage("insert.sql3",
                                make_table("COMPANY",
                                           make_column("ID", &Employee::id, primary_key()),
                                           make_column("NAME", &Employee::name),
                                           make_column("AGE", &Employee::age),
                                           make_column("ADDRESS", &Employee::address),
                                           make_column("SALARY", &Employee::salary)));
    storage.sync_schema();
    storage.remove_all<Employee>();

    Employee paul{-1, "Paul", 32, "California", 20000.00,};  // 插入Paul
    // 插入操作返回插入的ID
    paul.id = storage.insert(paul);

    storage.insert(Employee{-1, "Allen", 25, "Texas", 15000.00,}); // 插入Allen

    DetailedEmployee teddy;    // 插入Teddy
    teddy.name = "Teddy";
    teddy.age = 23;
    teddy.address = "Norway";
    teddy.salary = 20000.00;

    //  插入子类时需要指定基类
    teddy.id = storage.insert<Employee>(teddy);

    std::vector<Employee> employees;
    employees.push_back(Employee{-1, "Mark", 25, "Rich-Mond", 65000.00});
    employees.push_back(Employee{-1, "David", 27, "Texas", 85000.00});
    employees.push_back(Employee{-1, "Kim", 22, "South-Hall", 45000.00});

    //  transaction is optional. It is used here to optimize sqlite usage - every insert opens
    //  and closes database. So triple insert will open and close the db three times.
    //  Transaction openes and closes the db only once.
    storage.transaction([&] {
        for (auto &employee: employees)
        {
            storage.insert(employee);
        }
        return true;  //  commit
    });

    Employee james{-1, "James", 24, "Houston", 10000.00,};
    james.id = storage.insert(james);

    printf("--------------------\n");
    for (auto &employee: storage.iterate<Employee>())
    {
        printf("%s\n", storage.dump(employee).c_str());
    }

    //  INSERT INTO COMPANY(ID, NAME, AGE, ADDRESS, SALARY)
    //  VALUES (3, 'Sofia', 26, 'Madrid', 15000.0)
    //         (4, 'Doja', 26, 'LA', 25000.0)
    //  ON CONFLICT(ID) DO UPDATE SET NAME = excluded.NAME,
    //                                AGE = excluded.AGE,
    //                                ADDRESS = excluded.ADDRESS,
    //                                SALARY = excluded.SALARY
    storage.insert(
            into<Employee>(),
            columns(&Employee::id, &Employee::name, &Employee::age, &Employee::address, &Employee::salary),
            values(std::make_tuple(3, "Sofia", 26, "Madrid", 15000.0),
                   std::make_tuple(4, "Doja", 26, "LA", 25000.0)),
            on_conflict(&Employee::id).do_update(set(c(&Employee::name) = excluded(&Employee::name),
                                                     c(&Employee::age) = excluded(&Employee::age),
                                                     c(&Employee::address) = excluded(&Employee::address),
                                                     c(&Employee::salary) = excluded(&Employee::salary))));

    // 插入结果查询
    auto result = storage.select(asterisk<Employee>());
    fmt::print("{0}, {1}, {2}, {3}, {4}\n", "ID", "NAME", "AGE", "ADDRESS", "SALARY");
    for (auto &row: result)
    {
        fmt::print("{}\n", row);
    }
    std::cout << std::endl;

    return 0;
}