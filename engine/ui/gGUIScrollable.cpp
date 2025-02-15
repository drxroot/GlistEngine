/*
 * gGUIScrollable.cpp
 *
 *  Created on: Feb 5, 2022
 *      Author: noyan
 */

#include "gGUIScrollable.h"


gGUIScrollable::gGUIScrollable() {
	vsbw = 8;
	vsbh = height;
	hsbw = width;
	hsbh = 8;
	vsbx = right - vsbw;
	vsby = top;
	hsbx = left;
	hsby = bottom - hsbh;
	boxw = width;
	boxh = height;
	totalw = boxw;
	totalh = boxh;
	scrolldiff = 8;
	linehalf = scrolldiff / 2;
	vsbenabled = false;
	hsbenabled = false;
	sbenabled = false;
	vsbactive = true;
	hsbactive = true;
	iscursoronvsb = false;
	iscursoronhsb = false;
	sbbgcolor = middlegroundcolor;
	sbfgcolor = backgroundcolor;
	sbdragcolor.set(0, 0, 255, 255);
	vsbalpha = 1.0f;
	hsbalpha = 1.0f;
	sbalphaboxlimit = 1.0f;
//	sbalphaboxlimit = 0.2f;
	sbalphasblimit = 1.0f;
	alphablending = false;
	vrw = vsbw;
	vrh = vsbh;
	vrx = vsbx;
	vry = vsby;
	hrw = hsbw;
	hrh = hsbh;
	hrx = hsbx;
	hry = hsby;
	firstx = 0;
	firsty = 0;
	vsbmy = -1;
	hsbmx = -1;
	marginx = 0;
	marginy = 0;
	titlediff = 0;
	titlex = left;
	titley = top + font->getStringHeight("AE");
	titledy = font->getSize() * 1.8f;
	boxfbo = new gFbo();
	setTitleOn(false);
	isalpha = false;
}

gGUIScrollable::~gGUIScrollable() {
	delete boxfbo;
}

void gGUIScrollable::enableScrollbars(bool isVerticalEnabled, bool isHorizontalEnabled) {
	vsbenabled = isVerticalEnabled;
	hsbenabled = isHorizontalEnabled;
	if(vsbenabled || hsbenabled) sbenabled = true;
	else sbenabled = false;
}

void gGUIScrollable::setDimensions(int width, int height) {
	boxw = width;
	boxh = height;
	vsbh = height - hsbh;
	hsbw = width - vsbw;
	vsbx = boxw - vsbw;
	vsby = 0;
	hsbx = 0;
	hsby = boxh - hsbh;

	vrw = vsbw;
	vrh = vsbh * boxh / totalh;
	vrx = vsbx;
	vry = vsby;
	if(totalw != 0 && totalw != boxw) hrw = hsbw * boxw / totalw;
	else hrw = hsbw;
	hrh = hsbh;
	hrx = hsbx;
	hry = hsby;

	titlex = left + font->getStringWidth("i");
	titley = top + font->getStringHeight("AE");

	boxfbo->allocate(renderer->getWidth(), renderer->getHeight());

}

void gGUIScrollable::draw() {
	isalpha = renderer->isAlphaBlendingEnabled();
	if(isalpha) {
		renderer->disableAlphaTest();
		renderer->disableAlphaBlending();
	}
	renderer->setColor(fontcolor);
	if(istitleon) font->drawText(title, titlex, titley);
	renderer->setColor(0, 0, 0);
	boxfbo->bind();
	renderer->clearColor(0, 0, 0, 0);
	drawContent();
	drawScrollbars();
	boxfbo->unbind();
	renderer->setColor(255, 255, 255);
	boxfbo->drawSub(left, top + titledy, boxw, boxh, 0, renderer->getHeight() - boxh, boxw, boxh);
	renderer->setColor(foregroundcolor);
	gDrawRectangle(left, top + titledy, boxw, boxh, false);
	if(isalpha) {
		renderer->enableAlphaBlending();
		renderer->enableAlphaTest();
	}
}

void gGUIScrollable::drawContent() {
//	gLogi("Listbox") << "l:" << left << ", t:" << top << ", w:" << boxw << ", h:" << boxh;
//	renderer->setColor(textbackgroundcolor);
//	gDrawRectangle(0, 0, boxw, boxh, true);
}

void gGUIScrollable::drawScrollbars() {
	if(sbenabled) {
		gColor* oldcolor = renderer->getColor();
		alphablending = renderer->isAlphaBlendingEnabled();
		if(!alphablending) {
//			renderer->enableAlphaBlending();
//			renderer->enableAlphaTest();
		}

		if(vsbenabled) {
/*
			vsbalpha += 0.01f;
			if(!iscursoronvsb) {
				if(vsbalpha > sbalphaboxlimit) vsbalpha = sbalphaboxlimit;
			} else {
				vsbalpha += 0.04f;
				if(vsbalpha > sbalphasblimit) vsbalpha = sbalphasblimit;
			}
*/
//			sbbgcolor.a = vsbalpha;
			renderer->setColor(&sbbgcolor);
			gDrawRectangle(vsbx, vsby, vsbw, vsbh, true);

//			sbfgcolor.a = vsbalpha;
			renderer->setColor(&sbfgcolor);
			gDrawRectangle(vrx, vry, vrw, vrh, true);
		}

		if(hsbenabled) {
/*
			hsbalpha += 0.01f;
			if(!iscursoronhsb) {
				if(hsbalpha > sbalphaboxlimit) hsbalpha = sbalphaboxlimit;
			} else {
				hsbalpha += 0.04f;
				if(hsbalpha > sbalphasblimit) hsbalpha = sbalphasblimit;
			}
*/
			sbbgcolor.a = hsbalpha;
			renderer->setColor(&sbbgcolor);
			gDrawRectangle(hsbx, hsby - (titlediff * istitleon), hsbw, hsbh, true);

			sbfgcolor.a = hsbalpha;
			renderer->setColor(&sbfgcolor);
			gDrawRectangle(hrx, hry - (titlediff * istitleon), hrw, hrh, true);
		}

		if(!alphablending) {
//			renderer->disableAlphaTest();
//			renderer->disableAlphaBlending();
		}
		renderer->setColor(oldcolor);
	} else {
//		vsbalpha = 0.0f;
//		hsbalpha = 0.0f;
	}

	renderer->setColor(foregroundcolor);
	gDrawRectangle(vsbx, hsby - (titlediff * istitleon), vsbw, hsbh, true);
}

void gGUIScrollable::mouseMoved(int x, int y) {
	iscursoronvsb = false;
	iscursoronhsb = false;
	if(x >= left + vsbx && x < left + vsbx + vsbw && y >= top + titledy + vsby && y < top + titledy + vsby + vsbh) iscursoronvsb = true;
	if(x >= left + hsbx && x < left + hsbx + hsbw && y >= top + titledy + hsby - (titlediff * istitleon) && y < top + titledy + hsby - (titlediff * istitleon) + hsbh) iscursoronhsb = true;
}

void gGUIScrollable::mousePressed(int x, int y, int button) {
	if(vsbenabled && x >= left + vrx && x < left + vrx + vrw && y >= top + titledy + vry && y < top + titledy + vry + vrh) {
		vsbmy = y;
	}
	if(hsbenabled && x >= left + hrx && x < left + hrx + hrw && y >= top + titledy + hry - (titlediff * istitleon) && y < top + titledy + hry - (titlediff * istitleon) + hrh) {
		hsbmx = x;
	}
}

void gGUIScrollable::mouseDragged(int x, int y, int button) {
	if(vsbmy > -1) {
		vry += y - vsbmy;
		if(vry < 0) vry = 0;
		if(vry > boxh - vrh) vry = boxh - vrh;
		if(totalh < boxh) vry = vsby;

		firsty += (y - vsbmy) * vsbh / vrh;
		if(firsty < 0) firsty = 0;
		if(firsty > totalh - boxh + linehalf + (titlediff * istitleon) + marginy + hsbh) firsty = totalh - boxh + linehalf + (titlediff * istitleon) + marginy + hsbh;
		if(totalh < boxh) firsty = 0;

		vsbmy = y;
	}

	if(hsbmx > -1) {
		if(totalw == boxw) totalw = (boxw + marginx + vsbw) * 2;
		hrx += x - hsbmx;
		if(hrx < 0) hrx = 0;
		if(hrx > boxw - hrw) hrx = boxw - hrw;
		if(totalw < boxw) hrx = hsbx;

		firstx += (x - hsbmx) * hsbw / hrw;
		if(firstx < 0) firstx = 0;
		if(firstx > totalw - boxw + marginx + vsbw) firstx = totalw - boxw + marginx + vsbw;
		if(totalw < boxw) firstx = 0;

		hsbmx = x;
	}
}

void gGUIScrollable::mouseReleased(int x, int y, int button) {
	vsbmy = -1;
	hsbmx = -1;
}

void gGUIScrollable::mouseScrolled(int x, int y) {
	firsty -= y * scrolldiff;
	if(firsty < 0) firsty = 0;
	if(firsty > totalh - boxh + linehalf + (titlediff * istitleon) + marginy + hsbh) firsty = totalh - boxh + linehalf + (titlediff * istitleon) + marginy + hsbh;
	if(totalh < boxh) firsty = 0;
	if(vsbenabled && firsty >= 0) vry = firsty * (boxh - hsbh) / totalh;

	firstx -= x * scrolldiff;
	if(firstx < 0) firstx = 0;
	if(totalw == boxw || totalw == 0) totalw = (boxw + marginx + vsbw) * 2;
	if(totalw != boxw + marginx + vsbw - (2 * scrolldiff) && titlediff == 0) totalw = boxw + marginx + vsbw - (2 * scrolldiff);
	if(firstx > totalw - boxw + marginx + vsbw) firstx = totalw - boxw + marginx + vsbw;
	if(hsbenabled) hrx = firstx * (boxw - vsbw) / totalw;
}

void gGUIScrollable::windowResized(int w, int h) {
	delete boxfbo;
	boxfbo = new gFbo();
	setDimensions(boxw, boxh);
	gGUIControl::windowResized(w, h);
}

gFbo* gGUIScrollable::getFbo() {
	return boxfbo;
}

int gGUIScrollable::getTitleTop() {
	return titledy;
}

void gGUIScrollable::setMargin(float marginx, float marginy) {
	this->marginx = marginx;
	this->marginy = marginy;
}
