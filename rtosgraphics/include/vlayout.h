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
 * @file	/microtest1/rtosgraphics/include/vlayout.h/vlayout.h
 * @author	robin
 * @date	Apr 30, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_VLAYOUT_H_
#define INCLUDE_VLAYOUT_H_


#include <list>
#include "widget.h"
#include "canvas.h"
#include "inputevent.h"

class VLayout : public Widget
{
public:
	VLayout();

	void addWidget(Widget *widget, int stretch = 0);
	void addStretch(int stretch = 0);
	void removeWidget(Widget *widget);
	void draw(Canvas& canvas);

	int heightForWidth(int width) const;
	bool hasHeightForWidth() const;
	void processInputEvent(InputEvent *event);
	int marginBottom() const;
	void setMarginBottom(int marginBottom = 0);
	int marginLeft() const;
	void setMarginLeft(int marginLeft = 0);
	int marginRight() const;
	void setMarginRight(int marginRight = 0);
	int marginTop() const;
	void setMarginTop(int marginTop = 0);
	int spacing() const;
	void setSpacing(int spacing = 0);

	class LayoutItem
	{
		friend class VLayout;
	public:
		enum class LayoutItemType
		{
			Space,
			Stretch,
			Widget
		};

		LayoutItem(LayoutItemType type, Widget *item, int stretch) :
			m_type(type),
			m_stretch(stretch),
			m_calculatedHeight(0),
			m_cellHeight(0),
			m_item(item) { }

		LayoutItemType type() {
			return m_type;
		}

		Widget *widget() {
			return m_item;
		}

		int cellHeight() { return m_cellHeight; }
		void setCellHeight(int height) { m_cellHeight = height; }
		int stretch () { return m_stretch; }
		void setPreferredHeight(int height) { m_calculatedHeight = height; }
		int preferredHeight() { return m_calculatedHeight; }

		friend bool compare_widget_size(LayoutItem *left, LayoutItem *right);
		friend bool compare_widget_max_size(VLayout::LayoutItem *left, VLayout::LayoutItem *right);

	protected:
		LayoutItemType m_type;
		int m_stretch;
		int m_calculatedHeight;
		int m_cellHeight;
		Widget *m_item;
	};

protected:
	void setupGeometry();
	int m_geoCacheWidth;
	int m_geoCacheHeight;
	int m_spacing;
	int m_marginTop;
	int m_marginBottom;
	int m_marginLeft;
	int m_marginRight;

	std::list<LayoutItem*> m_items;
};


#endif /* INCLUDE_VLAYOUT_H_ */
