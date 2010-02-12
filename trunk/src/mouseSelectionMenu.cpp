#include "mouseSelectionMenu.h"

MouseSelectionMenu::MouseSelectionMenu(int x, int y)
:   gcn::Window("Mouse Selection Menu")
{
    try {
        /*_window = new gcn::Window("Mouse Selection Menu");
        _window->setPosition(x, y);
        _window->setOpaque(true);*/

        setPosition(x, y);
        setOpaque(true);
        setMovable(false);
        this->setTitleBarHeight(0);

        setVisible(true);
        resizeToContent();

        Gui::current().addWidget(this);
    } catch (gcn::Exception e) {
        cout << e.getMessage() << endl;
    }
}

MouseSelectionMenu::~MouseSelectionMenu()
{

}

void MouseSelectionMenu::addLabel(gcn::Label *label)
{
    label->setX(0);
    if(mWidgets.size() > 0)
        label->setY(mWidgets.back()->getY() + mWidgets.back()->getHeight());
    else
        label->setY(0);
    add(static_cast<gcn::Label*>(label));
    setHeight(getHeight() + label->getHeight());
    if(label->getWidth() > getWidth())
        setWidth(label->getWidth());
}

void MouseSelectionMenu::removeLabel(gcn::Label *label)
{
    /*remove(static_cast<gcn::Label*>(label));
    setHeight(getHeight() - label->gcn::Label::getHeight());*/
}

gcn::Rectangle MouseSelectionMenu::getChildrenArea()
{
    //int height = (mWidgets.front() == NULL) ? 0 : mWidgets.front()->getHeight();
    return gcn::Rectangle(0, 0, getWidth(), getHeight());
}

void MouseSelectionMenu::draw(gcn::Graphics* graphics)
{
    const gcn::Color &faceColor = getBaseColor();
    gcn::Color highlightColor, shadowColor;
    const int alpha = getBaseColor().a;
    highlightColor = faceColor + 0x303030;
    highlightColor.a = alpha;
    shadowColor = faceColor - 0x303030;
    shadowColor.a = alpha;

    gcn::Rectangle d = getChildrenArea();

    // Fill the background around the content
    graphics->setColor(faceColor);
    // Fill top
    graphics->fillRectangle(gcn::Rectangle(0,0,getWidth(),d.y - 1));
    // Fill left
    graphics->fillRectangle(gcn::Rectangle(0,d.y - 1, d.x - 1, getHeight() - d.y + 1));
    // Fill right
    graphics->fillRectangle(gcn::Rectangle(d.x + d.width + 1,
                                      d.y - 1,
                                      getWidth() - d.x - d.width - 1,
                                      getHeight() - d.y + 1));
    // Fill bottom
    graphics->fillRectangle(gcn::Rectangle(d.x - 1,
                                      d.y + d.height + 1,
                                      d.width + 2,
                                      getHeight() - d.height - d.y - 1));

    if (isOpaque())
    {
        graphics->fillRectangle(d);
    }

    // Construct a rectangle one pixel bigger than the content
    d.x -= 1;
    d.y -= 1;
    d.width += 2;
    d.height += 2;

    // Draw a border around the content
    graphics->setColor(shadowColor);
    // Top line
    graphics->drawLine(d.x,
                       d.y,
                       d.x + d.width - 2,
                       d.y);

    // Left line
    graphics->drawLine(d.x,
                       d.y + 1,
                       d.x,
                       d.y + d.height - 1);

    graphics->setColor(highlightColor);
    // Right line
    graphics->drawLine(d.x + d.width - 1,
                       d.y,
                       d.x + d.width - 1,
                       d.y + d.height - 2);
    // Bottom line
    graphics->drawLine(d.x + 1,
                       d.y + d.height - 1,
                       d.x + d.width - 1,
                       d.y + d.height - 1);

    /*drawChildren(graphics);*/

    /*int textX;
    int textY;

    textY = ((int)getTitleBarHeight() - getFont()->getHeight()) / 2;

    switch (getAlignment())
    {
      case gcn::Graphics::LEFT:
          textX = 4;
          break;
      case gcn::Graphics::CENTER:
          textX = getWidth() / 2;
          break;
      case gcn::Graphics::RIGHT:
          textX = getWidth() - 4;
          break;
      default:
          throw GCN_EXCEPTION("Unknown alignment.");
    }

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());
    graphics->pushClipArea(gcn::Rectangle(0, 0, getWidth(), getTitleBarHeight() - 1));
    graphics->drawText(getCaption(), textX, textY, getAlignment());
    graphics->popClipArea();*/

    graphics->pushClipArea(getChildrenArea());

    WidgetListIterator iter;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++) {
        gcn::Rectangle rec = (*iter)->getDimension();
        graphics->pushClipArea(rec);
        if((*iter)->getY() > drawRect.y && (*iter)->getY() < drawRect.y + drawRect.height) {
            //graphics->setColor(gcn::Color(0, 0, 0, 0));
            //graphics->fillRectangle(drawRect);
            gcn::Color prevColor = (*iter)->getForegroundColor();
            (*iter)->setForegroundColor(gcn::Color(255, 255, 255, 0));
            (*iter)->draw(graphics);
            (*iter)->setForegroundColor(prevColor);
        }
        else
            (*iter)->draw(graphics);
        graphics->popClipArea();
    }

    graphics->popClipArea();
}

void MouseSelectionMenu::mouseMoved(gcn::MouseEvent &mouseEvent)
{
    WidgetListIterator iter;
    for (iter = mWidgets.begin(); iter != mWidgets.end(); iter++) {
        if(mouseEvent.getY() > (*iter)->getY() && mouseEvent.getY() < (*iter)->getY() + (*iter)->getHeight()) {
            drawRect.x      = (*iter)->getX()       - 1;
            drawRect.y      = (*iter)->getY()       - 1;
            drawRect.width  = (*iter)->getWidth()   + 1;
            drawRect.height = (*iter)->getHeight()  + 1;
            return;
        }
    }
}
