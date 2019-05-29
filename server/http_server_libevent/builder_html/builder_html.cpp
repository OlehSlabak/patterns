#include "builder_html.h"

namespace Builder_1
{
    void CreatePage(PageBuilder& page)
    {
        page.CreadTitle();
        page.CreateBody();
        page.ClosePage();
    }
}

namespace Builder_2
{
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
}
