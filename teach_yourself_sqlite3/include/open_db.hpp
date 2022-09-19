////////////////////////////////////////////////////////////////////////
/// \file      open_db.hpp
/// \copyright Copyright (c) 2022 标准模型投资有限公司.
///            All rights reserved.
///            Licensed under the MIT License.
/// \author    金小海
/// \date      2022年09月18日, Sun, 12:52
/// \version   1.0
/// \brief
#ifndef OPEN_DB_HPP
#define OPEN_DB_HPP

#include "xhsqlite3.hpp"

using namespace xhsqlite3;

namespace xhsqlite3
{
    // 消费者
    struct Customers
    {
        std::string cust_id;
        std::string cust_name;
        std::string cust_address;
        std::string cust_city;
        std::string cust_state;
        std::string cust_zip;
        std::string cust_country;
        std::string cust_contact;
        std::unique_ptr<std::string> cust_email;
    };


    struct OrderItems
    {
        int order_num;
        int order_item;
        std::string prod_id;
        int quantity;
        double item_price;
    };


    struct Orders
    {
        int order_num;
        std::string order_date;
        std::string cust_id;
    };


    struct Products
    {
        std::string prod_id;
        std::string vend_id;
        std::string prod_name;
        double prod_price;
        std::string prod_desc;
    };


    // 供应商
    struct Vendors
    {
        std::string vend_id;       // 主键
        std::string vend_name;
        std::string vend_address;
        std::string vend_city;
        std::string vend_state;
        std::string vend_zip;
        std::string vend_country;
    };

    // 初始化数据库
    inline
    auto init_storage(const std::string &db_path)
    {
        auto
                storage = make_storage(db_path,
                                       make_table("Customers",
                                                  make_column("cust_id", &Customers::cust_id, xhsqlite3::primary_key()),
                                                  make_column("cust_name", &Customers::cust_name),
                                                  make_column("cust_address", &Customers::cust_address),
                                                  make_column("cust_city", &Customers::cust_city),
                                                  make_column("cust_state", &Customers::cust_state),
                                                  make_column("cust_zip", &Customers::cust_zip),
                                                  make_column("cust_country", &Customers::cust_country),
                                                  make_column("cust_contact", &Customers::cust_contact),
                                                  make_column("cust_email", &Customers::cust_email)),
                                       make_table("OrderItems",
                                                  make_column("order_num", &OrderItems::order_num),
                                                  make_column("order_item", &OrderItems::order_item),
                                                  make_column("prod_id", &OrderItems::prod_id),
                                                  make_column("quantity", &OrderItems::quantity),
                                                  make_column("item_price", &OrderItems::item_price)),
                                       make_table("Orders",
                                                  make_column("order_num", &Orders::order_num),
                                                  make_column("order_date", &Orders::order_date),
                                                  make_column("cust_id", &Orders::cust_id)),
                                       make_table("Products",
                                                  make_column("prod_id", &Products::prod_id, primary_key()),
                                                  make_column("vend_id", &Products::vend_id),
                                                  make_column("prod_name", &Products::prod_name),
                                                  make_column("prod_price", &Products::prod_price),
                                                  make_column("prod_desc", &Products::prod_desc)),
                                       make_table("Vendors",
                                                  make_column("vend_id", &Vendors::vend_id, primary_key()),
                                                  make_column("vend_name", &Vendors::vend_name),
                                                  make_column("vend_address", &Vendors::vend_address),
                                                  make_column("vend_city", &Vendors::vend_city),
                                                  make_column("vend_state", &Vendors::vend_state),
                                                  make_column("vend_zip", &Vendors::vend_zip),
                                                  make_column("vend_country", &Vendors::vend_country))
        );
        return storage;
    }
}

#endif //OPEN_DB_HPP
