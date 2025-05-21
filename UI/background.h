/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QLabel>
#include <QPixmap>
namespace Game {
	namespace UI {
		//战斗背景
		class BackGround: public QLabel {
		public:
			BackGround(QWidget* parent);
			BackGround(const QPixmap& bg, QWidget* parent);
			void SetBackGround(const QPixmap& bg);
		private:
			void Resize();
		protected:
			void resizeEvent(QResizeEvent* event) override;
			const QPixmap* pbg;
		};
	}
}