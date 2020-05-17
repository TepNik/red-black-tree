Implementation of Red-Black tree
======================================

This is the implementation of Red-Black tree on C++17. Here you can find template class rb_tree in namecpase TepNik.

This class supports move-semantics and also it is exception safe.

Also here you can find file for unit-testing in Visual Studio (CppUnitTestFramework).

# Public interface

### Template class rb_tree<Key_type, Value_type>

#### Constructor(default, copy, move) and destructor.

    rb_tree()
    rb_tree(const rb_tree &sec)
    rb_tree(rb_tree &&sec) noexcept
    ~rb_tree()

#### Erase function.

    void erase(const Key_type &key)

#### Insert function.

    void insert(Key_type key, Value_type value)

#### Copy and move operator.

    rb_tree& operator=(const rb_tree &sec)
    rb_tree& operator=(rb_tree &&sec) noexcept

#### Equal operator.

    bool operator==(const rb_tree &sec) const

#### Find function for const and non-const containers. Returns pointer to the value of this key if it is in the tree or nullptr if there is no such key in this tree.

    const Value_type* find(const Key_type &key) const
    Value_type* find(const Key_type &key)

#### Function that clean container.

    void clear()

#### Functions that return vectors of all keys and values in this tree.

    std::vector<Key_type> get_keys() const
    std::vector<Value_type> get_values() const

#### Function that print tree.

    void print(std::ostream &stream = std::cout, bool with_color = false) const

Tree from example

    L--key:4
       value:2
       +-------key:2
       |       value:2
       │       +-------key:1
       │       |       value:2
       │       L-------key:3
       │               value:2
       L-------key:6
               value:2
               +-------key:5
               |       value:2
               L-------key:8
                       value:2
                       +-------key:7
                       |       value:2
                       L-------key:9
                               value:2
                               L-------key:10
                                       value:2
#### Function that swap this tree with other.

    void swap(rb_tree &sec) noexcept

#### Function that returns number of elements in this tree.

    int size() const

# Example

### Compile test program
        make

### Run test program
        ./main


