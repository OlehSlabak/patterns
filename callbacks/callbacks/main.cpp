#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <map>
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <memory>
#include <cxxabi.h>

namespace function_pointer
{
    namespace for_each_mock
    {
        template<class InputIt, class UnaryFunction>
        UnaryFunction m_for_each(InputIt first, InputIt last, UnaryFunction f)
        {
              for (; first != last; ++first)
              {
                 f(*first);
              }
          return f;
        }

        void for_each_mock()
        {
            std::vector<double> v{1.2, 2.2, 4.0, 5.5, 7.2};
            double r = 4.0;
            m_for_each(v.begin(), v.end(), [&](double & v) { v += r; });
            m_for_each(v.begin(), v.end(), [](double & v) { std::cout << " " << v; });
            std::cout << std::endl;
        }
    }

    namespace game_key_mock
    {
        void player_jump()
        {
            std::cout << "Player jump" << std::endl;
        }
        void player_crouch();

        constexpr int total_keys()
        {
            return 2;
        }

        struct game_key_event
        {
            std::array<void(*)(), total_keys()> actions;

            void key_pressed(const unsigned key_id)
            {
                if (actions[key_id])
                    actions[key_id]();
            }

            void update_keybind(unsigned key_id, void(*new_actions)())
            {
                actions[key_id] = new_actions;
            }
        };


        void game_key_mock()
        {
            int key_id = 42;
            game_key_event key;
            key.update_keybind(key_id, &player_jump);
            key.key_pressed(key_id);
        }
    }

    namespace example_func_pointer
    {
        void tranform_every_int(int * v, unsigned n, int (*fp)(int))
        {
          for (unsigned i = 0; i < n; ++i)
          {
            v[i] = fp(v[i]);
          }
        }


        int double_int(int x) { return 2*x; }
        int square_int(int x) { return x*x; }


        void example_func_pointer()
        {
            int a[5] = {1, 2, 3, 4, 5};
            tranform_every_int(&a[0], 5, double_int); // now a == {2, 4, 6, 8, 10};
            for_each_mock::m_for_each(std::begin(a), std::end(a), [](int & a) { std::cout <<  a << " "; });
            std::cout << std::endl;
            tranform_every_int(&a[0], 5, square_int); // now a == {4, 16, 36, 64, 100};
            for_each_mock::m_for_each(std::begin(a), std::end(a), [](int & a) { std::cout <<  a << " "; });
        }

    }
}

namespace pointer_to_memeber
{
    struct MyStruct
    {
      int _y;
      int add(int x) const { return x+_y; }
      int mul(int x) const { return x*_y; }
      MyStruct(int y) : _y(y){};
    };

    namespace member_function
    {

      typedef int (MyStruct::* func_const_int_t) (int x) const;
      // The type of C_foo_p is a pointer to member function of C taking int returning int
      // Its value is initialized by a pointer to foo of C
      int (MyStruct::* C_foo_test1)(int) const = &MyStruct::add;
      func_const_int_t C_foo_test2 = &MyStruct::add; // // which can also be written using the typedef:

      int C_foobar(int x, MyStruct const &c, int (MyStruct::*moo)(int));

      int C_foobar(int x, MyStruct const &c, func_const_int_t moo)
      {
            int change = (c.*C_foo_test2)(x);
            std::cout << "Start y is [" << c._y << "] after change (call C_foo_test2 ==  MyStruct::add ~ 2 + 5) [" << change << "]\n";
            int return_value = (c.*moo)(change);
            std::cout << "Return value is [" << return_value << "]" << std::endl;
            return return_value;
      }

      void member_function()
      {
        MyStruct c(2);
        std::cout << "Add value \n";
        C_foobar(5, c, &MyStruct::add);
        std::cout << "Multiplication value \n";
        C_foobar(5, c, &MyStruct::mul);
      }
    }

    namespace std_function
    {
        // i.e. using the above function declaration of foo:

        int simple_function_one(int c)
        {
            int return_value = 0;
            std::cout << "Return from simple function one " << c << std::endl;
            return_value = c + 5;
            return return_value;
        }

        int simple_function_two(int c, int d)
        {
            int return_value = 0;
            std::cout << "Return from simple function two " << c << " " << d << std::endl;
            return_value = 9 * c + d;
            return return_value;
        }

        std::function<int(int)> stdf_simple = &simple_function_one;
        // or C::add:
        std::function<int(const MyStruct&, int)> stdf_MyStruct_add = &MyStruct::add;

        int stdf_simple_f(int x, std::function<int(int)> moo)
        {
            std::cout << " stdf_simple_f ";
            return x + moo(x); // std::function moo called
        }

        int stdf_MyStruct_f(int x, MyStruct const &c, std::function<int(MyStruct const &, int)> moo)
        {
            return x + moo(c, x); // std::function moo called using c and x
        }

        void std_function_test()
        {
            int test_value = 2;
            int test_res = 0;
            test_res = stdf_simple_f(test_value, &simple_function_one);
            std::cout << " stdf_simple_f " << test_res << std::endl;

            test_res = stdf_simple(6);
            std::cout << " stdf_simple " << test_res << std::endl;

            MyStruct c(2);
            test_res = stdf_MyStruct_f(test_value, c, &MyStruct::add);
            std::cout << " stdf_MyStruct_f  (MyStruct::add) " << test_res << std::endl;

            test_res = stdf_simple_f(test_value, [test_value](int x) -> int {return 5 * test_value; });
            std::cout << " stdf_simple_f  (lambda) " << test_res << std::endl;

            test_res = stdf_simple_f(test_value, std::bind(simple_function_two, 2, 3));
            std::cout << " stdf_simple_f  (bind 1) " << test_res << std::endl;

            test_res = stdf_simple_f(test_value, std::bind(simple_function_two, std::placeholders::_1, 3));
            std::cout << " stdf_simple_f  (bind 1 + place) " << test_res << std::endl;

            test_res = stdf_simple_f(test_value, std::bind(simple_function_two, 5, std::placeholders::_1));
            std::cout << " stdf_simple_f  (bind 2 + place) " << test_res << std::endl;
        }

        int double_int(int c)
        {
            return c*2;
        }

        void stdf_tranform_every_int(int *v, unsigned n, std::function<int(int)> fp)
        {
            for (unsigned i = 0; i < n; i++)
            {
                v[i] = fp(v[i]);
            }
        }

        int nine_x_and_y (int x, int y) { return 9*x + y; }
        void std_function()
        {
            int a[5] = {1,2,3,4,5};
            stdf_tranform_every_int(&a[0], 5, double_int);
            function_pointer::for_each_mock::m_for_each(std::begin(a), std::end(a), [](int & a) { std::cout << " " << a; });
            std::cout << std::endl;

            stdf_tranform_every_int(&a[0], 5, [](int x) -> int { return x/2; });
            function_pointer::for_each_mock::m_for_each(std::begin(a), std::end(a), [](int & a) { std::cout << " " << a; });
            std::cout << std::endl;

            stdf_tranform_every_int(&a[0], 5, std::bind(nine_x_and_y, std::placeholders::_1, 4));
            function_pointer::for_each_mock::m_for_each(std::begin(a), std::end(a), [](int & a) { std::cout << " " << a; });
            std::cout << std::endl;
        }

    }

}

namespace template_callback
{
    template<class InputIt, class OutputIt, class UnaryOperation>
    OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op)
    {
      while (first1 != last1)
      {
        *d_first++ = unary_op(*first1++);
      }
      return d_first;
    }
    template <class T>
    std::string type_name()
    {
      typedef typename std::remove_reference<T>::type TR;
      std::unique_ptr<char, void(*)(void*)> own (abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                                 nullptr, nullptr), std::free);
      std::string r = (own != nullptr) ? own.get() : typeid(TR).name();

      if (std::is_const<TR>::value)
      {
        r += " const";
      }
      if (std::is_volatile<TR>::value)
      {
        r += " volatile";
      }
      if (std::is_lvalue_reference<T>::value)
      {
        r += " &";
      }
      else if (std::is_rvalue_reference<T>::value)
      {
        r += " &&";
      }
      return r;
    }

    namespace simple_template
    {
        template<class R, class T>
        void stdf_transform_every_int_templ(int * v, unsigned const n,
                                            std::function<R(T)> fp)
        {
           for (unsigned i = 0; i < n; ++i)
           {
             v[i] = fp(v[i]);
           }
        }

        template<class F>
        void transform_every_int_templ(int * v, unsigned const n, F f)
        {
         std::cout << "transform_every_int_templ<"  << type_name<F>() << ">\n";
          for (unsigned i = 0; i < n; ++i)
          {
             v[i] = f(v[i]);
          }
        }
    }

    namespace example_template
    {
        int foo (int x) { return 2+x; }
        int foo_2 (int x, int y) { return 2*x*y; }
        int muh (int const &x) {return 3+x;}
        int & woof (int &x) { x *= 4; return x; }

        void print_int(int * p, unsigned const n)
        {
          bool f{ true };
          for (unsigned i = 0; i < n; ++i)
          {
            std::cout << (f ? "" : " ") << p[i];
            f = false;
          }
          std::cout << "\n";
        }


        void template_example()
        {

            int a[5] = { 1, 2, 3, 4, 5 };
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, foo);
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, muh);
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, woof);
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, [](int x) -> int { return x + x + x; });
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, std::bind(foo_2, std::placeholders::_1, 3));
            print_int(a, 5);
            simple_template::transform_every_int_templ(&a[0], 5, std::function<int(int)>{&foo});
            print_int(a, 5);
        }

    }
}

void print_menu(const std::map<std::string, std::function<void()>> &menu)
{
    int i = 0;
    for (auto element : menu)
    {
        std::cout << i << ") "<< element.first << "\n";
        i++;
    }

    /*

    struct menu_item
{
  virtual ~menu_item() {}
  virtual std::string item_text() const = 0;
  virtual void go() = 0;
};

struct print_hello_item
{
  std::string item_text() const { return "display greeting"; }
  void go() { std::cout << "Hello there, Mr. User."; }
};

struct kill_everyone_item
{
  std::string item_text() const { return "Go on murderous rampage"; }
  void go() { for(;;) kill_the_world(); }
};

struct menu_menu_item
{
  menu_menu_item(std::string const& text) : text_(text), items() {}
  void add_item(std::unique_ptr<menu_item> item) { items.push_back(std::move(item)); }
  void go()
  {
    std::cout << "Choose: \n";
    std::for_each(items.begin(), items.end(), [](std::unique_ptr<menu_item> const& item)
    {
      std::cout << "\t" << item->item_text() << "\n";
    });
    std::cout << "\n\n\tYour choice: ";
    int choice = get_number_from_console();
    if (items.size() > choice) items[choice]->go();
  }
  std::string item_text() const { return text_; }

private:
  std::string text_;
  std::vector<std::unique_ptr<menu_item> > items;
};
  print_menu(menu);
  menu_menu_item top_item;
  top_item.add(std::unique_ptr<menu_item>(new print_hello_item));
  top_item.add(std::unique_ptr<menu_item>(new kill_everyone_item));

  top_item.go();

    */

}


namespace lambda
{
    namespace functor_test
    {
        struct f {
            void operator()(int)
            {
                // do something
            }
        };
        void func(std::vector<int>& v)
        {
            f f;
            function_pointer::for_each_mock::m_for_each(v.begin(), v.end(), f);
        }
    }

    namespace lambda_test
    {
        void func1(std::vector<int>& v)
        {
            std::for_each(v.begin(), v.end(), [](int) { /* do something here*/ });
        }

        void func2(std::vector<double>& v)
        {
            std::transform(v.begin(), v.end(), v.begin(), [](double d) { return d < 0.000001 ? 0 : d; });
        }

        void func3(std::vector<double>& v)
        {
            std::transform(v.begin(), v.end(), v.begin(), [](double d) -> double {
                if (d < 0.00001)
                    return 0;
                else
                    return d;
            });
        }

        void func4(std::vector<double>& v, const double& epsilon)
        {
            std::transform(v.begin(), v.end(), v.begin(),
                           [epsilon](double d) -> double {
                                if (d < epsilon)
                                    return 0;
                                else
                                    return d;
                           });
        }

        void func5(std::vector<double>& v)
        {
            std::transform(v.begin(), v.end(), v.begin(),
                           [epsilon = 2.0](double d) -> double {
                                if (d < epsilon)
                                    return 0;
                                else
                                    return d;
                           });
        }

        class Student
        {
            public:
                int age;
                void captureByThis()
                {
                    auto myLamba = [this] () {
                        /* return this->age */
                        return age;
                    };
                }
        };


        static int num = 4;
        auto whatIsNum = []() {return num;};

        /* In C++14 and above, use a return type of auto */
        auto makeAutoAgeChecker(const int requiredAge)
        {
            return [=](const int age) { return age > requiredAge; };
        }
        /* Below C++14, use std::function to safely wrap the lambda */
        std::function <bool(int)> makeFuncAgeChecker(const int requiredAge)
        {
            return [=](const int age) { return age > requiredAge; };
        }

        auto autoAgeChecker = makeAutoAgeChecker(18);
        auto functionAgeChecker = makeFuncAgeChecker(42);

        constexpr auto Square = [] (int n) { return n*n; };
//        static_assert(Square(2) == 4, "Yes");
//        static_assert(Square(2) != 4, "No, this value should be equal");

        template<typename Range, typename Func, typename T>
        constexpr T SimpleAccumulate(const Range& range, Func func, T init)
        {
            for (auto &&elem : range)
            {
                init += func(elem);
            }

            return init;
        }


        void lambda_test()
        {
            std::vector<double> vd = {1.0, 3.4, 2.5, 3.5, 1.9};
            func4(vd, 3.0);
            function_pointer::for_each_mock::m_for_each(std::begin(vd), std::end(vd), [](double & a) { std::cout << " " << a; });
            func5(vd);
            function_pointer::for_each_mock::m_for_each(std::begin(vd), std::end(vd), [](double & a) { std::cout << " " << a; });
            std::cout  << std::endl;
            std::cout << autoAgeChecker(15) << std::endl; // false
            std::cout << functionAgeChecker(51) << std::endl; // true
            std::cout << "___________________" << std::endl;

            int toMutate = 1;
            auto lamda1 = [=]() mutable // capture by value
            {
                toMutate = 2;
                std::cout << toMutate << std::endl;
            };
            lamda1();

            std::cout << toMutate << std::endl;

            auto lamda2 = [&]()  // capture by value
            {
                toMutate = 3;
                std::cout << toMutate << std::endl;
            };
            lamda2();
            std::cout << toMutate << std::endl;

            int x = 5;
            int y = 5;
            [&]() { ++x; ++y; }();
            std::cout << x << " " << y << std::endl;


            constexpr std::array arr{1,2,3};
          //  static_assert(SimpleAccumulate(arr, [](int i){ return i*i;}, 0) == 14);
          //  static_assert(SimpleAccumulate(arr, [](int i){ return i*i;}, 0) == 15, "NONONO");

        }
    }

}

//int main()
auto main() -> int
{
    std::map<std::string, std::function<void()>> menu;

   /* menu["for_each_example"] = function_pointer::for_each_mock::for_each_mock;
    menu["game_key_example"] = function_pointer::game_key_mock::game_key_mock;
    menu["func_pointer_example"] = function_pointer::example_func_pointer::example_func_pointer;
    menu["member_function_example"] = pointer_to_memeber::member_function::member_function;
    menu["std_function_test_example"] = pointer_to_memeber::std_function::std_function_test;
    menu["std_function_example"] = pointer_to_memeber::std_function::std_function;*/
    menu["template_example"] = template_callback::example_template::template_example;
    menu["lambda_example"] = lambda::lambda_test::lambda_test;

    for (auto element : menu)
    {
        std::cout << "\n" << element.first << ": \n\n";
        element.second();
        std::cout << std::endl;
        std::cout << std::string(25, '-')<< std::endl;
    }

    return 0;
}
