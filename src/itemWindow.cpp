#include "itemWindow.h"
#include "gui.h"
#include "renderer.h"

#include <cstdlib>

ItemWindow::ItemWindow(int x, int y, Inventory inventory) : gcn::Window("Items")
{
    _inventory = inventory;

    setPosition(x, y);
    setSize(275, 125);
    setVisible(true);

    itemmap items = _inventory.getItems();

    for(unsigned i = 0; i < items.size(); i++) {
        div_t divresult;
        divresult = div (i, 5);

        Vector2D pos(divresult.rem * 50 + getX() + 20, divresult.quot * 50 + getY() + 30);
        gcn::Label *tempLabel = new gcn::Label(items[i].getName());
        tempLabel->setPosition(pos.x, pos.y);
        _labels.push_back(tempLabel);
        add(tempLabel);
    }

    Gui::current().addWidget(this);
}

ItemWindow::~ItemWindow()
{
    while(_labels.size() > 0) {
        delete _labels.back();
        _labels.pop_back();
    }
}

void ItemWindow::draw(gcn::Graphics* graphics)
{
    gcn::Window::draw(graphics);

    itemmap items = _inventory.getItems();

    //graphics->pushClipArea(getChildrenArea());

    for(unsigned i = 0; i < items.size(); i++) {
        div_t divresult;
        divresult = div (i, 5);

        Vector2D pos(divresult.rem * 50 + getX() + 20, divresult.quot * 50 + getY() + 20);
        Renderer::current().drawImage(items[i].getImage().get(), pos);
    }

    //graphics->popClipArea();
}
