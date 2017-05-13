/**
 * microtest1
 * ----------------------------------------
 *
 * MIT License
 *
 * Copyright (c) 2017 robin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file	/microtest1/rtosgraphics/src/vlayout.cpp/vlayout.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */

#include "vlayout.h"

VLayout::VLayout() :
	m_geoCacheWidth(0),
	m_geoCacheHeight(0),
	m_marginTop(0),
	m_marginBottom(0),
	m_marginLeft(0),
	m_marginRight(0),
	m_spacing(0)
{

}

void VLayout::addWidget(Widget *widget, int stretch)
{
	LayoutItem *item = new LayoutItem(LayoutItem::LayoutItemType::Widget, widget, stretch);
	m_items.push_back(item);
}

void VLayout::removeWidget(Widget *widget)
{
	std::list<LayoutItem*>::const_iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;
		if (item->m_item == widget) {
			m_items.remove(item);
			return;
		}
	}
}

void VLayout::processInputEvent(InputEvent *event)
{
	std::list<LayoutItem*>::const_iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;
		if (item->type() == LayoutItem::LayoutItemType::Widget) {
			int widget_x1 = item->widget()->x();
			int widget_x2 = item->widget()->width() + widget_x1;
			int widget_y1 = item->widget()->y();
			int widget_y2 = item->widget()->height() + widget_y1;

			if (widget_x1 <= event->x() && event->x() <= widget_x2) {
				if (widget_y1 <= event->y() && event->y() <= widget_y2) {
					item->widget()->processInputEvent(event);
					return;
				}
			}
		}
	}
}

void VLayout::draw(Canvas& canvas)
{
	setupGeometry();

	unsigned int x = this->x() + m_marginLeft;
	unsigned int y = this->y() + m_marginTop;
	std::list<LayoutItem*>::const_iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;

		switch (item->type()) {
			case LayoutItem::LayoutItemType::Widget:
			{
				int cell_height = item->cellHeight();
				int widget_height = item->widget()->height();
				int widget_width = item->widget()->width();

				if (widget_height < cell_height) {
					item->widget()->setY(y + ((cell_height - widget_height) / 2));
				}
				else {
					item->widget()->setY(y);
				}

				if (widget_width < width()) {
					item->widget()->setX(x + (((width() - m_marginLeft - m_marginRight) - widget_width) / 2));
				}
				else {
					item->widget()->setX(x);
				}

				item->widget()->draw(canvas);
				y += cell_height + m_spacing;
			}
		}
	}
}

//
// sort widget max sizes from low to high
//
bool compare_widget_max_size(VLayout::LayoutItem *left, VLayout::LayoutItem *right)
{
	return (left->widget()->maxHeight() > right->widget()->maxHeight());
}

//
// sort widget preferred sizes from high to low
//
bool compare_widget_size(VLayout::LayoutItem *left, VLayout::LayoutItem *right)
{
	return (left->preferredHeight() < left->preferredHeight());
}

void VLayout::setupGeometry()
{
	//
	// if the width and height are unchanged, don't re-caclulate
	//
	if (width() == m_geoCacheWidth && height() == m_geoCacheHeight) {
		return;
	}

	m_geoCacheWidth = width();
	m_geoCacheHeight = height();

	int contents_height = height() - m_marginTop - m_marginBottom - ((m_items.size() - 1) * m_spacing);
	int contents_width = width() - m_marginLeft - m_marginRight;
	int min_height = 0;
	int widgets_expanding = 0;
	int widgets_expanding_max_height = 0;	// total max height
	int widgets_preferred = 0;
	int widgets_preferred_height = 0;
	int widgets_preferred_max_height = 0;
	int widgets_fixed_height = 0;
	int widgets_fixed = 0;
	int widget_total_stretch = 0;
	int stretches = 0;

	std::list<LayoutItem*> items_expanding;
	std::list<LayoutItem*> items_preferred;
	std::list<LayoutItem*>::const_iterator iter;

	//
	// this first iteration is used to collect information
	// about all the widgets and put together some aggregate
	// information to be used in the actual geometry calculations
	// down below
	//
	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item->type() == LayoutItem::LayoutItemType::Widget) {
		}

		switch (item->type()) {
			case LayoutItem::LayoutItemType::Widget:
			{
				int widget_preferred_height;

				if (item->widget()->hasHeightForWidth()) {
					widget_preferred_height = item->widget()->heightForWidth(width());
					if (widget_preferred_height == -1) {
						widget_preferred_height = item->widget()->heightHint();
					}
				}
				else {
					widget_preferred_height = item->widget()->heightHint();
				}

				min_height += widget_preferred_height;
				item->setPreferredHeight(widget_preferred_height);

				switch (item->widget()->verticalSizePolicy()) {
					case Widget::SizePolicy::ExpandingSizePolicy:
						widgets_expanding++;
						widget_total_stretch += item->stretch();
						widgets_expanding_max_height += item->widget()->maxHeight();
						items_expanding.push_back(item);
						break;
					case Widget::SizePolicy::PreferredSizePolicy:
						widgets_preferred++;
						widgets_preferred_height += item->preferredHeight();
						widgets_preferred_max_height += item->widget()->maxHeight();

						//
						// if this item has stretch, it should be treated
						// as an expanding item
						//
						if (item->stretch() > 0) {
							items_expanding.push_back(item);
						}
						else {
							items_preferred.push_back(item);
						}
						break;
					case Widget::SizePolicy::FixedSizePolicy:
					case Widget::SizePolicy::MinimumSizePolicy:
						widgets_fixed++;
						widgets_fixed_height += item->preferredHeight();
						break;
				}

				break;
			}
			case LayoutItem::LayoutItemType::Stretch:
			{
				stretches += 1;
				break;
			}
		}
	}

	//
	// here we should check for an overage, which means we need
	// to adjust to get everything to fit, starting with widgets
	// that can shrink. if no widgets can shrink, then....
	// - shrink all widgets according to their preferred:total ratio
	// - don't strink widgets, allow them to be pushed off-screen
	// - something else???
	//
	if (min_height > contents_height) {
		// TODO: re-adjust sizing
	}

	//
	// area into which expanding widgets are divided, according to
	// stretch factor
	//
	int expanding_area = contents_height - widgets_preferred_height - widgets_fixed_height;
	int expanding_area_remaining = expanding_area;

	//
	// sort the list of expanding widgets from lowest max size to highest
	// to make sure that smallest widgets leave extra space for the larger
	// expanding widgets
	//
	items_expanding.sort(compare_widget_max_size);

	for (iter = items_expanding.begin(); iter != items_expanding.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item->stretch() == 0) {
			if (widget_total_stretch == 0) {
				int h = expanding_area_remaining / widgets_expanding;
				int mh = item->widget()->maxHeight();

				//
				// leave expanding widgets blank until the very end,
				// we really don't need to calculate their size yet...
				// unless it maxes out, in which case cap it there
				//
				if (mh < h) {
					h = mh;
				}

				item->widget()->setHeight(h);
				item->setCellHeight(h);
				widgets_expanding--;
				expanding_area_remaining -= h;
			}
			else {
				//
				// this item will act like it has preferred sizing
				//
				item->widget()->setHeight(item->preferredHeight());
				item->setCellHeight(item->preferredHeight());
				widgets_expanding--;
				expanding_area_remaining -= item->preferredHeight();
			}
		}
		else {
			int widget_height = (expanding_area_remaining / widget_total_stretch) * item->stretch();
			int widget_max_height = item->widget()->maxHeight();

			if (widget_max_height < widget_height) {
				widget_height = widget_max_height;
			}

			item->widget()->setHeight(widget_height);
			item->setCellHeight(widget_height);
			widget_total_stretch -= item->stretch();
			expanding_area_remaining -= widget_height;
		}
	}

	//
	// any leftover space in the expanding area means that all the
	// widgets had a maximum size that was less than allowed, so
	// we give that space now to the preferred widgets in the hopes
	// that they will use it
	//
	int remaining_height = widgets_preferred_height + expanding_area_remaining;
	int remaining_widgets = widgets_preferred;

	//
	// sort the list of preferred widgets from largest to smallest, to
	// make sure that larger widgets are given enough room and smaller
	// widgets share the remaining room equally
	//
	items_preferred.sort(compare_widget_size);

	for (iter = items_preferred.begin(); iter != items_preferred.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item->preferredHeight() > (remaining_height / remaining_widgets)) {
			// widget is too big for equally-divided space, so give it just enough
			item->widget()->setHeight(item->preferredHeight());
			item->setCellHeight(item->preferredHeight());
			remaining_height -= item->preferredHeight();
		}
		else {
			// widgets from here on out will fit within equally-divided spaces
			int h = remaining_height / remaining_widgets;
			item->widget()->setHeight(h);
			item->setCellHeight(h);
			remaining_height -= h;
		}
		remaining_widgets--;
	}

	//
	// if after all that we still have space left, then we evenly divide the
	// space up between the cells, since we can assume at this point that all
	// widgets are at their maximum size, so we basically pad
	//
	int item_padding = remaining_height / m_items.size();

	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item_padding > 0) {
			int cell_height = item->cellHeight();
			item->setCellHeight(cell_height + item_padding);
		}

		if (item->type() == LayoutItem::LayoutItemType::Widget) {
			switch (item->widget()->horizontalSizePolicy()) {
				case Widget::SizePolicy::ExpandingSizePolicy:
				case Widget::SizePolicy::PreferredSizePolicy:
				{
					if (item->widget()->maxWidth() < contents_width) {
						item->widget()->setWidth(item->widget()->maxWidth());
					}
					else {
						item->widget()->setWidth(contents_width);
					}
					break;
				}
				case Widget::SizePolicy::FixedSizePolicy:
				{
					item->widget()->setWidth(item->widget()->widthHint());
					break;
				}
				case Widget::SizePolicy::MinimumSizePolicy:
				{
					if (item->widget()->widthHint() > contents_width) {
						item->widget()->setWidth(contents_width);
					}
					else {
						item->widget()->setWidth(item->widget()->widthHint());
					}
					break;
				}
			}
		}
	}
}

bool VLayout::hasHeightForWidth() const
{
	bool hfw = false;
	std::list<LayoutItem*>::const_iterator iter;

	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item->type() == LayoutItem::LayoutItemType::Widget) {
			hfw = hfw || item->widget()->hasHeightForWidth();
		}
	}

	return hfw;
}

int VLayout::heightForWidth(int width) const
{
	int height = 0;
	std::list<LayoutItem*>::const_iterator iter;

	if (!hasHeightForWidth()) {
		return -1;
	}

	for (iter = m_items.begin(); iter != m_items.end(); ++iter) {
		LayoutItem *item = *iter;

		if (item->widget()->hasHeightForWidth()) {
			height += item->widget()->heightForWidth(width);
		}
		else {
			height += item->widget()->heightHint();
		}
	}

	return height;
}

int VLayout::marginBottom() const {
	return m_marginBottom;
}

void VLayout::setMarginBottom(int marginBottom) {
	m_marginBottom = marginBottom;
}

int VLayout::marginLeft() const {
	return m_marginLeft;
}

void VLayout::setMarginLeft(int marginLeft) {
	m_marginLeft = marginLeft;
}

int VLayout::marginRight() const {
	return m_marginRight;
}

void VLayout::setMarginRight(int marginRight) {
	m_marginRight = marginRight;
}

int VLayout::marginTop() const {
	return m_marginTop;
}

void VLayout::setMarginTop(int marginTop) {
	m_marginTop = marginTop;
}

int VLayout::spacing() const {
	return m_spacing;
}

void VLayout::setSpacing(int spacing) {
	m_spacing = spacing;
}
