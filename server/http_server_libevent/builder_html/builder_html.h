#ifndef BUILDER_HTML_H_INCLUDED
#define BUILDER_HTML_H_INCLUDED
//#include <csignal>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>
#include "../singleton_logging/logging.hpp"


/* builder*/

class PageBuilder
{
    public:
        virtual ~PageBuilder() {};
        virtual void CreadTag() = 0;
        virtual void CreateBody() = 0;
        virtual void CloseTag() = 0;

        virtual void Print()
        {
             logging::DEBUG("create element");
            // print to file
             std::cout << _oss.str() << std::endl;
        };
    protected:
        std::ostringstream _oss;
        std::string str;
};



class HtmlElement : public PageBuilder
{
    public:
        virtual ~HtmlElement() {};
        HtmlElement(const std::string &name, const std::string &text, const int indent) : _name(name), _text(text), _current_indent(indent) {}

        virtual void CreadTag()
        {
            _oss << std::string(_base_indet_size * (_current_indent + 1), ' ') << "<" << _name << ">" << std::endl;
        }

        virtual void CreateBody()
        {
            if (_text.size() > 0)
                _oss << std::string(_base_indet_size * (_current_indent + 2), ' ') << _text << std::endl;
        }
        virtual void CloseTag()
        {
            _oss << std::string(_base_indet_size * (_current_indent + 1), ' ') << "</" << _name << ">" << std::endl;
        }
    private:
        std::string _name;
        std::string _text;
        const size_t _base_indet_size = 2;
        int _current_indent = 0;
};

/*
class JavaElement : public PageBuilder
{
    public:
        virtual ~JavaElement() {};
        JavaElement(const std::string &name, const std::string &text, const int indent) : _name(name), _text(text), _current_indent(indent) {}
        virtual void CreadTag(const int indent) const
        {
            _index(_base_indet_size * (_current_indent + 1), ' ');
            _oss << _index << "<" << _name << ">" << std::endl;
        }

        virtual void CreateBody()
        {
            if (_text.size() > 0)
                _oss << std::string(_base_indet_size * (_current_indent + 1), ' ') << _text << std::endl;
        }
        virtual void CloseTag()
        {
             oss << _index << "</" << _name << ">" << std::endl;
        }
    private:
        std::string _text;
        std::string _name;
        std::string _index;
        const size_t _base_indet_size = 2;
        int _current_indent = 0;
};

*/

void CreatePage(PageBuilder& page)
{
    page.CreadTag();
    page.CreateBody();
    page.CloseTag();
}









 /*   HtmlBuilder builder { "ul" };
    builder.add_child("li", "hello").add_child("li", "world");
    std::cout << builder.str() << std::endl;*/

/*
std::string HtmlElement::str(int indent) const
{
    std::ostringstream oss;
    std::string i(_indet_size * indent, ' ');
    oss << i << "<" << _name << ">" << std::endl;
    if (_text.size() > 0)
        oss << std::string(_indet_size * (indent + 1), ' ') << _text << std::endl;

    for (const auto& e : _elements)
        oss << e.str(indent + 1);

    oss << i << "</" << _name << ">" << std::endl;
    return oss.str();
}

HtmlBuilder& HtmlBuilder::add_child(std::string child_name, std::string child_text)
{
    logging::DEBUG("create html element" + child_name);
    HtmlElement e{child_name, child_text};
    _root._elements.emplace_back(e);
    return *this;
}
*/







/*

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


*/










#endif // BUILDER_HTML_H_INCLUDED
