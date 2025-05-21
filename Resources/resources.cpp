/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "resources.h"
#include <QPainter>
#include <QDir>
#include "../define.h"
#include "gifmanage.h"
namespace Game {
	QMap<RoleResources, QMap<ActionType, SharedGifFrameManager*>> Util::rolegifs;
	QMap<RoleResources, QString> Util::rolename = {
		{BaShen,"bashen"}
	};
	QMap<KeyActionType, Qt::Key> Util::keysmap = {
		{MoveLeft,Qt::Key_A},{MoveRight,Qt::Key_D}, {MoveDown,Qt::Key_S},
		{MoveUp,Qt::Key_W}
	};//映射玩家键盘的行动
	QList<QPixmap*> Util::bgs;
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
			for(const QFileInfo &file: files) {
				SharedGifFrameManager*movie = new SharedGifFrameManager(file.absoluteFilePath());
				if (file.baseName().toLower() == "default") {
					rolegifs[key][ActionType::Default] = movie;
				}
				else if (file.baseName().toLower() == "forward") {
					rolegifs[key][ActionType::Forward] = movie;
				}
				else if (file.baseName().toLower() == "back") {
					rolegifs[key][ActionType::Back] = movie;
					
				}
				else if (file.baseName().toLower() == "run") {
					rolegifs[key][ActionType::Run] = movie;

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

}