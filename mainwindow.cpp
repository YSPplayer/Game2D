/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "mainwindow.h"
#include "Resources/resources.h"
#include "UI/scene.h"
namespace Game {
    namespace Client {
        MainWindow::MainWindow(QWidget* parent)
            : QWidget(parent) {
            ui.setupUi(this);
            Init();
            resize(1920, 1040);
            engine = new GameEngine(this);
            pscene = new Scene(this);
            pscene->SetBackGround(Util::bgs[0]);
            pscene->setGeometry(0, 0, width(),height());
            BindEvent();
            engine->Start();
        }

        MainWindow::~MainWindow()
        {}

        /// <summary>
        /// 游戏循环
        /// </summary>
        void MainWindow::GameLoop() {
            pscene->Update();
        }

        /// <summary>
        /// 初始化
        /// </summary>
        void MainWindow::Init() {
            Util::LoadResources();
        }
        void MainWindow::BindEvent() {
            connect(engine,&GameEngine::update,this,&MainWindow::GameLoop);
        }
    }
}