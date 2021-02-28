#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <gl\GL.h>
#pragma comment(lib, "opengl32.lib")

namespace MenuGL {

	class CDraw {
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;

		CDraw(unsigned char r, unsigned char g, unsigned char b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
		
		void FillRectangle(float x1, float y1, float x2, float y2) {
			::glBegin(GL_POLYGON);
			::glColor3ub(this->r, this->g, this->b);
			::glVertex2f(x1, y1);
			::glVertex2f(x2, y1);
			::glVertex2f(x2, y2);
			::glVertex2f(x1, y2);
			::glEnd();
		}
		
		void Rectangle(float x1, float y1, float x2, float y2) {
			::glBegin(GL_LINES);
			::glColor3ub(this->r, this->g, this->b);
			::glVertex2f(x1, y1);
			::glVertex2f(x2, y1);
			::glVertex2f(x2, y1);
			::glVertex2f(x2, y2);
			::glVertex2f(x2, y2);
			::glVertex2f(x1, y2);
			::glVertex2f(x1, y2);
			::glVertex2f(x1, y1);
			::glEnd();
		}

		void Line(float x1, float y1, float x2, float y2) {
			::glBegin(GL_LINES);
			::glColor3ub(this->r, this->g, this->b);
			::glVertex2f(x1, y1);
			::glVertex2f(x2, y2);
			::glEnd();
		}
	};

	class CFont {
	public:
		unsigned int base;
		unsigned char r;
		unsigned char g;
		unsigned char b;

		CFont(int size, unsigned char r, unsigned char g, unsigned char b) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->base = ::glGenLists(96);

			HDC hdc = ::wglGetCurrentDC();
			HFONT hFont = ::CreateFontA(-size, NULL, NULL, NULL, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
			HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

			::wglUseFontBitmapsA(hdc, 32, 96, this->base);
			::SelectObject(hdc, hOldFont);
			::DeleteObject(hFont);
		}

		void Print(float x, float y, const char *format, ...) {
			::glColor3ub(this->r, this->g, this->b);
			::glRasterPos2f(x, y);

			char text[100];
			::va_list args;

			va_start(args, format);
			::vsprintf_s(text, 100, format, args);
			va_end(args);

			::glPushAttrib(GL_LIST_BIT);
			::glListBase(this->base - 32);
			::glCallLists(::strlen(text), GL_UNSIGNED_BYTE, text);
			::glPopAttrib();
		}
	};

	class CMenu {
	public:

		class CItem {
		public:
			std::string name;
			bool active;

			CItem(std::string name) {
				this->name = name;
				this->active = false;
			}
		};

		class CTab {
		public:
			std::string name;
			std::vector<MenuGL::CMenu::CItem> items;

			CTab(std::string name) {
				this->name = name;
			}
		};

		bool show = false;
		float x, y, w, h;
		std::vector<MenuGL::CMenu::CTab> tabs;
		int current_tab = 0;
		int current_item = 0;

		CMenu(float x, float y, float w, float h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}

		void ProcessInput(void) {

			if (::GetAsyncKeyState(VK_LEFT) & 1) {
				if (this->current_tab == 0)
					this->current_tab = this->tabs.size() - 1;
				else
					this->current_tab--;
				this->current_item = 0;
			}

			if (::GetAsyncKeyState(VK_RIGHT) & 1) {
				if (this->current_tab == (this->tabs.size() - 1))
					this->current_tab = 0;
				else
					this->current_tab++;
				this->current_item = 0;
			}

			if (::GetAsyncKeyState(VK_UP) & 1) {
				if (this->current_item == 0)
					this->current_item = this->tabs[this->current_tab].items.size() - 1;
				else
					this->current_item--;
			}

			if (::GetAsyncKeyState(VK_DOWN) & 1) {
				if (this->current_item == (this->tabs[this->current_tab].items.size() - 1))
					this->current_item = 0;
				else
					this->current_item++;
			}

			if ((::GetAsyncKeyState(VK_RETURN) & 1) /*&& (::GetKeyState(VK_RETURN) & 8000)*/)
				this->tabs[this->current_tab].items[this->current_item].active = !this->tabs[this->current_tab].items[this->current_item].active;
		}

		void Draw(void) {
			MenuGL::CFont font(14, 150, 150, 150);
			MenuGL::CFont selected_font(14, 100, 50, 250);
			MenuGL::CDraw tab(50, 50, 50);
			MenuGL::CDraw selected_tab(65, 65, 65);

			float step = this->w / this->tabs.size();
			for (int i = 0; i < this->tabs.size(); ++i) {

				if (this->current_tab == i) {
					selected_font.Print(this->x + step*i + 5.f + (step - 7.f*this->tabs[i].name.length()) / 2.f, this->y + 23.f, this->tabs[i].name.c_str());
					selected_tab.FillRectangle(this->x + step*i + 5.f, this->y + 5.f, this->x + step*(i + 1), this->y + 35.f);
				}
				else {
					font.Print(this->x + step*i + 5.f + (step - 7.f*this->tabs[i].name.length()) / 2.f, this->y + 23.f, this->tabs[i].name.c_str());
					tab.FillRectangle(this->x + step*i + 5.f, this->y + 5.f, this->x + step*(i + 1), this->y + 35.f);
				}
			}

			MenuGL::CDraw inactive_button(250, 80, 50);
			MenuGL::CDraw active_button(120, 220, 90);

			for (int i = 0; i < this->tabs[this->current_tab].items.size(); ++i) {

				if (this->tabs[this->current_tab].items[i].active)
					active_button.FillRectangle(this->x + 10.f, this->y + 47.f + i*20.f, this->x + 20.f, this->y + 47.f + i*20.f + 10.f);
				else
					inactive_button.FillRectangle(this->x + 10.f, this->y + 47.f + i*20.f, this->x + 20.f, this->y + 47.f + i*20.f + 10.f);

				if (this->current_item == i)
					selected_font.Print(this->x + 25.f, this->y + 54.f + i*20.f, this->tabs[this->current_tab].items[i].name.c_str());
				else
					font.Print(this->x + 25.f, this->y + 54.f + i*20.f, this->tabs[this->current_tab].items[i].name.c_str());
			}

			MenuGL::CDraw window(20, 20, 20);
			window.FillRectangle(this->x, this->y, this->x + this->w + 5, this->y + this->h);
		}
	};
};
