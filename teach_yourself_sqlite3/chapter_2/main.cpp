////////////////////////////////////////////////////////////////////////
/// \file      main.cpp
/// \copyright Copyright (c) 2022 标准模型投资有限公司.
///            All rights reserved.
///            Licensed under the MIT License.
/// \author    金小海
/// \date      2022年09月18日, Sun, 12:48
/// \version   1.0
/// \brief

#include <iostream>

#include "xhsqlite3.hpp"

#include "open_db.hpp"

using namespace xhsqlite3;

int main(int argc, char **argv)
{
    auto storage = init_storage("../db/teach_yourself_sqlite3.sql3");
    // storage.sync_schema();

    // 检索单个列
    auto prod_name = storage.select(&Products::prod_name);
    printf("%-20s\n", "prod_name");
    for (auto &name: prod_name)
    {
        printf("%-20s\n", name.c_str());
    }
    std::cout << std::endl;


    return 0;
}