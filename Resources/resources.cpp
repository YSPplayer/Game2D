/*
  �����ˣ�YSP
  �������ڣ�2025-05-19
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
	};//ӳ����Ҽ��̵��ж�
	QList<QPixmap*> Util::bgs;
	/// <summary>
	/// ������Դ
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool Util::LoadResources() {
		//���ر���ͼƬ
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
		//�������ﶯ���ز�
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
	/// �����ǰ��ɫ����࣬ǰ��Ӧ�����Ҽ���������Ҳ���ǰ��Ӧ�������
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
	/// ͼƬ��չ�㷨 ���沿��
	/// </summary>
	/// <param name="original"></param>
	/// <param name="targetHeight"></param>
	/// <returns></returns>
	QPixmap Util::ExtendPixmapHeight(const QPixmap& original, qint32 targetHeight) {
		if (original.height() >= targetHeight || original.isNull()) {
			return original;
		}
		// �����»�������Ȳ��䣬�߶�ΪĿ��ֵ��
		QPixmap newPixmap(original.width(), targetHeight);
		newPixmap.fill(Qt::transparent); // ͸������
		// ���»����ϻ���
		QPainter painter(&newPixmap);
		// ��ԭͼ���Ƶ��»����ײ���y���� = �ܸ߶� - ԭͼ�߶ȣ�
		painter.drawPixmap(0, targetHeight - original.height(), original);
		return newPixmap;
	}

	/// <summary>
	/// ͼ��תΪ����ͼƬ
	/// </summary>
	/// <param name="original"></param>
	/// <param name="mode"></param>
	/// <returns></returns>
	QPixmap Util::ToMirrorPixmap(const QPixmap& original, FlipMode mode) {
		if (original.isNull()) return QPixmap();

		QTransform transform;
		switch (mode) {
		case HorizontalFlip: //ˮƽ
			transform.scale(-1, 1);
			break;
		case VerticalFlip: //��ֱ
			transform.scale(1, -1);
			break;
		case BothFlip: //ȫ��
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
	/// ����2ΪС�� ȥβ��
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	double Util::TruncateToTwoDecimal(double value) {
		// �ȳ���100��ȡ�����ٳ���100
		return std::floor(value * 100) / 100;
	}

}