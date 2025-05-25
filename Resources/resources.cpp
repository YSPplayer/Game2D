/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "resources.h"
#include <QPainter>
#include <QDir>
#include <QLabel>
#include "../define.h"
#include "gifmanage.h"
namespace Game {
	QMap<RoleResources, QMap<ActionType, SharedGifFrameManager*>> Util::rolegifs;
	QMap<RoleResources, QString> Util::rolename = {
		{BaShen,"bashen"}
	};
	QMap<KeyActionType, Qt::Key> Util::keysmap = {
		{MoveLeft,Qt::Key_A},{MoveRight,Qt::Key_D}, {MoveDown,Qt::Key_S},
		{MoveUp,Qt::Key_W},{MoveAttack,Qt::Key_J}
	};//映射玩家键盘的行动
	QMap<ActionType, QString> Util::acitonname = {
		{Default,"default"},{Forward,"forward"},{Back,"back"},{Run,"run"},{Attack,"attack"}
	};
	QList<QPixmap*> Util::bgs;
	double Util::deltaTime = 0.0;
	double Util::lastTime = 0.0;
	/// <summary>
	/// 加载资源
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool Util::LoadResources() {
		//加载背景图片
		QString bgPath = GAME_RESOURCES_PATH + "/Bg";
		QDir dir(bgPath);
		if (dir.exists()) {
			QList<QString> bgfilters;
			bgfilters << "*.png";
			QFileInfoList files = dir.entryInfoList(bgfilters, QDir::Files);
			for (const QFileInfo& file : files) {
				bgs.push_back(new QPixmap(file.absoluteFilePath()));
			}
		}
		//加载人物动作素材
		QString rolepath = GAME_RESOURCES_PATH + "/Role";
		for(const RoleResources& key : rolename.keys()) {
			const QString& dirname = rolename[key];
			dir = QDir(rolepath + "/" + dirname);
			if (!dir.exists()) continue;
			rolegifs[key].clear();
			QList<QString> filters;
			filters << "*.gif";
			QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
			QList<ActionType> atypes = { Default ,Forward,Back,Run,Attack };
			for (qint32 i = atypes.count() - 1; i >= 0; --i) {
				ActionType atype = atypes[i];
				const auto& _files = ListFilter::Where<QFileInfo>(files, [atype](const QFileInfo& info)->bool {
					return info.baseName().toLower() == acitonname[atype];
				});
				if (_files.count() > 0) {
					SharedGifFrameManager* movie = new SharedGifFrameManager(_files[0].absoluteFilePath());
					rolegifs[key][atype] = movie;
					atypes.removeAt(i);
				}
			}
			//剩下的在目录中寻找
			for (const ActionType atype : atypes) {
				dir = QDir(rolepath + "/" + dirname + "/" + acitonname[atype]);
				if (dir.exists()) {
					QList<QString> filters;
					filters << "*.png";
					QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
					QList<QPixmap> maps;
					for (auto& info : files) {
						maps.push_back(QPixmap(info.absolutePath() + "/" + info.fileName()));
					}
					SharedGifFrameManager* movie = new SharedGifFrameManager(maps);
					rolegifs[key][atype] = movie;
				}
			}
		}
		return true;
	}

	/// <summary>
	/// 如果当前角色在左侧，前进应该是右键，如果在右侧则前进应该是左键
	/// </summary>
	/// <param name="type"></param>
	/// <param name="left"></param>
	/// <returns></returns>
	ActionType Util::KeyActionTypeToActionType(KeyActionType type, bool left) {
		if (left) {
			if (type == KeyActionType::MoveLeft) return ActionType::Back;
			if (type == KeyActionType::MoveRight || type == KeyActionType::MoveUp
				|| type == KeyActionType::MoveDown) return ActionType::Forward;
			
		}
		else {
			if (type == KeyActionType::MoveRight) return ActionType::Back;
			if (type == KeyActionType::MoveLeft || type == KeyActionType::MoveUp
				|| type == KeyActionType::MoveDown) return ActionType::Forward;
		}
		if (type == KeyActionType::MoveRun) return ActionType::Run;
		return ActionType::Default;
	}

	/// <summary>
	/// 图片扩展算法 上面部分
	/// </summary>
	/// <param name="original"></param>
	/// <param name="targetHeight"></param>
	/// <returns></returns>
	QPixmap Util::ExtendPixmapHeight(const QPixmap& original, qint32 targetHeight) {
		if (original.height() >= targetHeight || original.isNull()) {
			return original;
		}
		// 创建新画布（宽度不变，高度为目标值）
		QPixmap newPixmap(original.width(), targetHeight);
		newPixmap.fill(Qt::transparent); // 透明背景
		// 在新画布上绘制
		QPainter painter(&newPixmap);
		// 将原图绘制到新画布底部（y坐标 = 总高度 - 原图高度）
		painter.drawPixmap(0, targetHeight - original.height(), original);
		return newPixmap;
	}

	/// <summary>
	/// 图像转为镜像图片
	/// </summary>
	/// <param name="original"></param>
	/// <param name="mode"></param>
	/// <returns></returns>
	QPixmap Util::ToMirrorPixmap(const QPixmap& original, FlipMode mode) {
		if (original.isNull()) return QPixmap();

		QTransform transform;
		switch (mode) {
		case HorizontalFlip: //水平
			transform.scale(-1, 1);
			break;
		case VerticalFlip: //垂直
			transform.scale(1, -1);
			break;
		case BothFlip: //全部
			transform.scale(-1, -1);
			break;
		}

		QPixmap mirrored(original.size());
		mirrored.fill(Qt::transparent);

		QPainter painter(&mirrored);
		painter.setTransform(transform);
		painter.drawPixmap(
			mode == HorizontalFlip ? -original.width() : 0,
			mode == VerticalFlip ? -original.height() : 0,
			original
		);

		return mirrored;
	}

	/// <summary>
	/// 保留2为小数 去尾法
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	double Util::TruncateToTwoDecimal(double value) {
		// 先乘以100，取整后再除以100
		return std::floor(value * 100) / 100;
	}

	double Util::GetTrueSpeed(double speed) {
		return speed * deltaTime;
	}

	QPoint Util::FindFirstNonTransparentPixel(const QLabel* label) {
		// 获取label在父组件中的位置
		QPoint labelPos = label->pos();
		// 获取QLabel上的图片
		const QPixmap& pixmap = label->pixmap();
		if (!pixmap) {
			return labelPos;  // 如果没有图片，返回原点
		}
		// 将QPixmap转换为QImage以便访问像素数据
		const QImage& image = pixmap.toImage();
		// 遍历图片的每个像素
		for (qint32 y = 0; y < image.height(); ++y) {
			for (qint32 x = 0; x < image.width(); ++x) {
				// 获取当前像素点的颜色
				QColor color = image.pixelColor(x, y);
				// 检查alpha通道值是否大于0
				if (color.alpha() > 0) {
					// 找到第一个非透明像素
					// 计算这个点在父组件中的实际位置
					int actualX = labelPos.x() - x;
					int actualY = labelPos.y() - y;
					return QPoint(actualX, actualY);
				}
			}
		}
		return labelPos;
	}

}