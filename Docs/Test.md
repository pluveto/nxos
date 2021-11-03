# 测试

## 单元测试
针对函数单元进行测试，测试函数的基本功能，稳定性，健壮性。
着重点：参数，返回值，功能。

```c
TEST(TEST_UnitName)
{
    ASSERT_TRUE
    ASSERT_FALSE

    EXCEPT_TRUE
    EXCEPT_FALSE
    {
        /* UnitSetdown */
    }
}

TEST_UNITS(test_case_name)
{
    TEST_UNIT(TEST_UnitName),
    TEST_UNIT_U(TEST_UnitName, Setup),
    TEST_UNIT_D(TEST_UnitName, Setdown),
    TEST_UNIT_UD(TEST_UnitName, Setup, Setdown),
};

TEST_SETUP(test_case_name)
{

}

TEST_CLEANUP(test_case_name)
{

}

DEFINE_TEST(test_case_name);
```


## 自测试
针对用户接口做测试，测试接口，数据传递等。
着重点：参数，返回值，功能。

## 集成测试
把多个单元组合在一起，测试一个全局的功能，一些局部测试难以发现的问题，就容易在集成测试中被发现。
着重点：功能的组合，效果的实现，压力测试，性能测试。


