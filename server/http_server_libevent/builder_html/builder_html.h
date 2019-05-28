#ifndef BUILDER_HTML_H_INCLUDED
#define BUILDER_HTML_H_INCLUDED
//#include <csignal>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <iomanip>
#include "../singleton_logging/logging.hpp"


/* builder*/

////// Product declarations and inline impl. (possibly Product.h) //////

namespace Builder_1
{
    class PageBuilder
    {
        public:
            virtual ~PageBuilder() {};
            virtual void CreadTitle() = 0;
            virtual void CreateBody() = 0;
            virtual void ClosePage() = 0;

            virtual void Print()
            {
                 std::cout << _oss.str() << std::endl;
            };
        protected:
            std::ostringstream _oss;
    };

    class HtmlElement : public PageBuilder
    {
        public:
            virtual ~HtmlElement() {};
            HtmlElement(const std::string &name, const std::string &text, const int indent) : _name(name), _text(text), _current_indent(indent) {}

            virtual void CreadTitle()
            {
                logging::DEBUG("create tag element :" + _name);
                _oss << std::string(_base_indet_size * (_current_indent + 1), ' ') << "<" << _name << ">" << std::endl;
            }

            virtual void CreateBody()
            {
                if (_text.size() > 0)
                    _oss << std::string(_base_indet_size * (_current_indent + 2), ' ') << _text << std::endl;
            }
            virtual void ClosePage()
            {
                _oss << std::string(_base_indet_size * (_current_indent + 1), ' ') << "</" << _name << ">" << std::endl;
            }
        private:
            std::string _name;
            std::string _text;
            const size_t _base_indet_size = 2;
            int _current_indent = 0;
    };

    class TextElement : public PageBuilder
    {
        public:
            virtual ~TextElement() {};
            TextElement(const std::string &name, const std::string &text) : _name(name), _text(text) {}

            virtual void CreadTitle()
            {
                logging::DEBUG("create text :" + _name);
                _oss << _name << std::endl;
            }

            virtual void CreateBody()
            {
                if (_text.size() > 0)
                    _oss << _text << std::endl;
            }
            virtual void ClosePage()
            {
                auto now = std::chrono::system_clock::now();
                auto now_c = std::chrono::system_clock::to_time_t(now);
                _oss << "Time now : " << std::put_time(std::localtime(&now_c), "%c")  << std::endl;
            }
        private:
            std::string _name;
            std::string _text;
    };


    void CreatePage(PageBuilder& page);
}


namespace Builder_2
{
    struct HtmlBuilder;

    struct HtmlElement
    {
        std::string _name;
        std::string _text;
        std::vector<HtmlElement> _elements;

        const size_t _indet_size = 2;


        std::string str(int indent = 0) const;
        static std::unique_ptr<HtmlBuilder> build(std::string root_name)
        {
            return std::make_unique<HtmlBuilder>(root_name);
        }
        private :
            friend class HtmlBuilder;
            HtmlElement() {}
            HtmlElement(const std::string &name, const std::string &text) : _name(name), _text(text) {}
    };

    struct HtmlBuilder
    {
        HtmlBuilder(std::string root_name)
        {
            _root._name = root_name;
        }

        HtmlBuilder& add_child(std::string child_name, std::string child_text);
        HtmlElement _root;
        std::string str() { return _root.str();}
        operator HtmlElement() const {return _root; }
    };
}

namespace Builder_3
{
    class Product{
        public:
            // use this class to construct Product.
            class Builder;

        private:
            // variables in need of initialization to make valid object
            const int i;
            const float f;
            const char c;

            // Only one simple constructor - rest is handled by Builder
            Product( const int i, const float f, const char c ) : i(i), f(f), c(c){}

        public:
            // Product specific functionality
            void print() {
                    std::cout << "Product internals dump:" << std::endl;
                    std::cout << "i: " << i << std::endl;
                    std::cout << "f: " << f << std::endl;
                    std::cout << "c: " << c << std::endl;
            }
            void doSomething();
            void doSomethingElse();
    };


    class Product::Builder{
        private:
            int i;
            float f;
            char c;

            // default values for variables
            static constexpr int defaultI = 1;
            static constexpr float defaultF = 3.1415f;
            static constexpr char defaultC = 'a';

        public:
            // create Builder with default values assigned
            // (in C++11 they can be simply assigned above on declaration instead)
            Builder() : i( defaultI ), f( defaultF ), c( defaultC ){ }

            // sets custom values for Product creation
            // returns Builder for shorthand inline usage (same way as cout <<)
            Builder& setI( int i ){ this->i = i; return *this; }
            Builder& setF( float f ){ this->f = f; return *this; }
            Builder& setC( char c ){ this->c = c; return *this; }

            // prepare specific frequently desired Product
            // returns Builder for shorthand inline usage (same way as cout <<)
            Builder& setProductP(){
                i = 42;
                f = -1.0f/12.0f;
                c = '@';

                return *this;
            }

            // produce desired Product
            Product build(){
                // Here, optionaly check variable consistency
                // and also if Product is buildable from given information
                return Product( i, f, c );
            }
    };
}







#endif // BUILDER_HTML_H_INCLUDED
