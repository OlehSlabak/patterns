#include <iostream>
#include <vector>
#include <array>
#include <functional>


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

            void key_pressed(unsigned key_id)
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





int main()
{

   /* function_pointer::for_each_mock::for_each_mock();
      function_pointer::game_key_mock::game_key_mock();
      function_pointer::example_func_pointer::example_func_pointer();

*/
/*
      pointer_to_memeber::member_function::member_function();
*/
      //pointer_to_memeber::std_function::std_function_test();
        pointer_to_memeber::std_function::std_function();


    return 0;
}
