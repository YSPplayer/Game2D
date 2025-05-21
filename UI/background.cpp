/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "background.h"
namespace Game {
	namespace UI {
		BackGround::BackGround(QWidget* parent) :QLabel(parent) {
			this->pbg = nullptr;
		}
		BackGround::BackGround(const QPixmap& bg, QWidget* parent):QLabel(parent) {
			SetBackGround(bg);
		}
		void BackGround::SetBackGround(const QPixmap& bg) {
			this->pbg = new QPixmap(bg);
			Resize();
		}

		void BackGround::Resize() {
			if (!this->pbg || this->pbg->isNull()) return;
			setPixmap(this->pbg->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		}

		void BackGround::resizeEvent(QResizeEvent* event) {
			Resize();
		}
	}
}