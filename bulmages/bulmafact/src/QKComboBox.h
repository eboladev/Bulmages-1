// Keyed Combo Box

class QKComboBox : public QComboBox {

	Q_OBJECT

	QKComboBox(QWidget * parent = 0, const char * name = 0);
	~QKComboBox();

private:
	QMap mapa(QString, QString);

public:
	void insertItem(QString, QString);
	virtual void clear();
	virtual void removeItem(int);

}
