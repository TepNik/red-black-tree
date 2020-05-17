#include "CppUnitTest.h"
#include "rb_tree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TepNik;

namespace tests
{
    TEST_CLASS(tests){
    public :
        TEST_METHOD(insert_in_empty_container)
        {
            rb_tree<int, int> tree;
            tree.insert(1, 2);
            std::vector<int> keys{1}, values{2};
            Assert::IsTrue(tree.get_keys() == keys && tree.get_values() == values);
        }
        TEST_METHOD(test_copy_constructor)
        {
            rb_tree<int, int> tree1;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            rb_tree<int, int> tree2 = tree1;
            Assert::IsTrue(tree1.get_keys() == tree2.get_keys() && tree1.get_values() == tree2.get_values());
        }
        TEST_METHOD(erase_element)
        {
            rb_tree<int, int> tree;
            tree.insert(1, 2);
            tree.insert(2, 2);
            tree.insert(3, 2);
            tree.erase(2);
            std::vector<int> keys{1, 3}, values{2, 2};
            Assert::IsTrue(tree.get_keys() == keys && tree.get_values() == values);
        }
        TEST_METHOD(test_copy_operator)
        {
            rb_tree<int, int> tree1;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            rb_tree<int, int> tree2;
            tree2 = tree1;
            Assert::IsTrue(tree1.get_keys() == tree2.get_keys() && tree1.get_values() == tree2.get_values());
        }
        TEST_METHOD(test_equal_operator)
        {
            rb_tree<int, int> tree1, tree2;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            tree2.insert(1, 2);
            tree2.insert(2, 2);
            tree2.insert(3, 2);
            Assert::IsTrue(tree1 == tree2);
        }
        TEST_METHOD(test_find_function)
        {
            rb_tree<int, int> tree1;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            int *temp = tree1.find(1);
            Assert::IsTrue(temp && *temp == 2);
            temp = tree1.find(5);
            Assert::IsTrue(temp == nullptr);
        }
        TEST_METHOD(test_get_keys_values)
        {
            rb_tree<int, int> tree1;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            auto keys_t = tree1.get_keys();
            auto values_t = tree1.get_values();
            std::vector<int> keys{1, 2, 3}, values{2, 2, 2};
            Assert::IsTrue(keys_t == keys && values_t == values);
        }
        TEST_METHOD(test_size_function)
        {
            rb_tree<int, int> tree1;
            tree1.insert(1, 2);
            tree1.insert(2, 2);
            tree1.insert(3, 2);
            tree1.erase(1);
            tree1.erase(5);
            Assert::IsTrue(tree1.size() == 2);
        }
    };
}