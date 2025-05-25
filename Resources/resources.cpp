/*
  �����ˣ�YSP
  �������ڣ�2025-05-19
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
	};//ӳ����Ҽ��̵��ж�
	QMap<ActionType, QString> Util::acitonname = {
		{Default,"default"},{Forward,"forward"},{Back,"back"},{Run,"run"},{Attack,"attack"}
	};
	QList<QPixmap*> Util::bgs;
	double Util::deltaTime = 0.0;
	double Util::lastTime = 0.0;
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
			//ʣ�µ���Ŀ¼��Ѱ��
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

	double Util::GetTrueSpeed(double speed) {
		return speed * deltaTime;
	}

	QPoint Util::FindFirstNonTransparentPixel(const QLabel* label) {
		// ��ȡlabel�ڸ�����е�λ��
		QPoint labelPos = label->pos();
		// ��ȡQLabel�ϵ�ͼƬ
		const QPixmap& pixmap = label->pixmap();
		if (!pixmap) {
			return labelPos;  // ���û��ͼƬ������ԭ��
		}
		// ��QPixmapת��ΪQImage�Ա������������
		const QImage& image = pixmap.toImage();
		// ����ͼƬ��ÿ������
		for (qint32 y = 0; y < image.height(); ++y) {
			for (qint32 x = 0; x < image.width(); ++x) {
				// ��ȡ��ǰ���ص����ɫ
				QColor color = image.pixelColor(x, y);
				// ���alphaͨ��ֵ�Ƿ����0
				if (color.alpha() > 0) {
					// �ҵ���һ����͸������
					// ����������ڸ�����е�ʵ��λ��
					int actualX = labelPos.x() - x;
					int actualY = labelPos.y() - y;
					return QPoint(actualX, actualY);
				}
			}
		}
		return labelPos;
	}

}