#include "mainwindow.h"
#include <QSurfaceFormat>

MainWindow::MainWindow()
	:
	_settings()
{

	QWidget* widgetPtr = new QWidget;
	this->setCentralWidget(widgetPtr);

	const QSurfaceFormat glFormat;

	this->_oglWidgetPtr = new OGLWidget;
	this->_oglWidgetPtr->setFormat(glFormat);
	this->_oglWidgetPtr->setSizePolicy(
		QSizePolicy::Expanding,
		QSizePolicy::Expanding
	);

	//PushButtons
	this->_smoothBtPtr = new QPushButton;
	this->_smoothBtPtr->setText(tr("Smoothing"));
	this->_smoothBtPtr->setMaximumHeight(30);
	this->_smoothBtPtr->setMaximumWidth(120);
	QObject::connect(this->_smoothBtPtr, SIGNAL(clicked()),this,SLOT(smoothCloud()));

	this->_thinBtPtr = new QPushButton;
	this->_thinBtPtr->setText(tr("Thinning"));
	this->_thinBtPtr->setMaximumHeight(30);
	this->_thinBtPtr->setMaximumWidth(120);
	QObject::connect(this->_thinBtPtr, SIGNAL(clicked()), this, SLOT(thinCloud()));


	//SpinBoxes
	this->_smoothFactorSbPtr = new QDoubleSpinBox;
	this->_thinRadiusSbPtr = new QDoubleSpinBox;
	QObject::connect(this->_smoothFactorSbPtr, SIGNAL(valueChanged(double)), this, SLOT(changeSmoothFactor(double)));
	QObject::connect(this->_thinRadiusSbPtr, SIGNAL(valueChanged(double)), this, SLOT(changeThinRadius(double)));


	//ListView of visible Data
	this->_visibleElementsScrollWidgetPtr = new QListWidget;
	this->_visibleElementsScrollWidgetPtr->setMaximumWidth(120);
	QObject::connect(this->_visibleElementsScrollWidgetPtr, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(repaintOGLWidget(QListWidgetItem*)));

	//Layouts
	QVBoxLayout* settingsLayoutPtr = new QVBoxLayout;
	settingsLayoutPtr->setAlignment(Qt::AlignTop);
	settingsLayoutPtr->addWidget(this->_thinRadiusSbPtr);
	settingsLayoutPtr->addWidget(this->_thinBtPtr);
	settingsLayoutPtr->addWidget(this->_smoothFactorSbPtr);
	settingsLayoutPtr->addWidget(this->_smoothBtPtr);
	settingsLayoutPtr->addWidget(this->_visibleElementsScrollWidgetPtr);

	QHBoxLayout* layoutPtr = new QHBoxLayout;
	layoutPtr->setMargin(5);
	layoutPtr->addWidget(this->_oglWidgetPtr);
	layoutPtr->addLayout(settingsLayoutPtr);

	widgetPtr->setLayout(layoutPtr);

	this->_createActions();
	this->_createMenus();

	this->setWindowTitle(tr("I3DS"));
	this->setMinimumSize(160, 160);
	this->resize(600, 400);
	
}

void MainWindow::changeSmoothFactor(double value) {
	_settings.smoothFactor = value;
}

void MainWindow::changeThinRadius(double value) {
	_settings.thinRadius = value;
}

void MainWindow::editSettings()
{
	SettingsWindow setWin(this);
	setWin.exec();
	_settings = setWin.getContainer();
}

void MainWindow::loadFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\Windows\\", tr("xyz Files (*.xyz)"));
	if (!fileName.isNull()) {
		// inform Controller:
		this->_onRequestLoadFile(fileName.toStdString());
	}
}

void MainWindow::repaintOGLWidget(QListWidgetItem* sender) {
	this->_onRequestUpdateOGLWidget();
}

void MainWindow::updateProjectionModelView(ModelProperties& props) {
	this->_oglWidgetPtr->updateProjectionModelView(props);
	this->_oglWidgetPtr->update();
}

void MainWindow::thinCloud(){
	this->_onRequestThinCloud();
}

void MainWindow::smoothCloud() {
	this->_onRequestSmoothCloud();
}

void MainWindow::render(std::vector<std::shared_ptr<PointCloud3d>>& visibleElements) {

		//send render info to widget
		this->_oglWidgetPtr->render(visibleElements);
		//this->_pollInteractionsWithWindow();
}

void MainWindow::render(int r, int g, int b) {

	//send render info to widget
	this->_oglWidgetPtr->render(r,g,b);
	//this->_pollInteractionsWithWindow();
}

void MainWindow::setOnRequestLoadFile(std::function<void(std::string)> callback)
{
	this->_onRequestLoadFile = callback;
}

void MainWindow::setOnRequestThinCloud(std::function<void()> callback)
{
	this->_onRequestThinCloud = callback;
}

void MainWindow::setOnRequestSmoothCloud(std::function<void()> callback)
{
	this->_onRequestSmoothCloud = callback;
}

void MainWindow::setOnRequestUpdateOGLWidget(std::function<void()> callback)
{
	this->_onRequestUpdateOGLWidget = callback;
}

void MainWindow::addVisibleElementToList()
{
	QListWidgetItem* newListWidgetPtr = new QListWidgetItem(this->_visibleElementsScrollWidgetPtr);
	newListWidgetPtr->setFlags(newListWidgetPtr->flags() | Qt::ItemIsUserCheckable);
	newListWidgetPtr->setCheckState(Qt::Unchecked);
	QString text = QString("PointCloud %1").arg(this->_visibleElementsScrollWidgetPtr->count());
	newListWidgetPtr->setText(text);
	this->_visibleElementsScrollWidgetPtr->addItem(newListWidgetPtr);
}

std::vector<int> MainWindow::getVisibleElementsIndices()
{
	std::vector<int> indicesVector;
	int size = this->_visibleElementsScrollWidgetPtr->count();
	for (int i = 0; i < size; i++) {
		if (this->_visibleElementsScrollWidgetPtr->item(i)->checkState() == Qt::Checked){
			indicesVector.push_back(i);
		}
	}
	return indicesVector;
}

void MainWindow::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_oglWidgetPtr->setOnRequestPaintGL(callback);
}

void MainWindow::closeApplication() {
	std::cout << "invoked closeApplication" << std::endl;
}

void MainWindow::_createActions() {

	this->_createLoadFileAction();
	this->_createCloseApplicationAction();
	this->_createEditSettingsAction();

}

void MainWindow::_createLoadFileAction() {

	this->loadFileAction = new QAction(tr("&load file"), this);
	// TODO: setShortcuts
	this->loadFileAction->setStatusTip(tr("TODO"));
	this->connect(
		this->loadFileAction,
		&QAction::triggered,
		this,
		&MainWindow::loadFile
	);
}

void MainWindow::_createCloseApplicationAction() {

	this->closeApplicationAction = new QAction(tr("&close application"), this);
	// TODO: setShortcuts
	this->closeApplicationAction->setStatusTip(tr("TODO"));
	this->connect(
		this->closeApplicationAction,
		&QAction::triggered,
		this,
		&MainWindow::closeApplication
	);
}

void MainWindow::_createEditSettingsAction() {

	this->editSettingsAction = new QAction(tr("&edit Settings"), this);
	this->editSettingsAction->setStatusTip(tr("TODO"));
	this->connect(
		this->editSettingsAction,
		&QAction::triggered,
		this,
		&MainWindow::editSettings
	);
}

void MainWindow::_createMenus() {
	this->fileMenu = this->menuBar()->addMenu(tr("&File"));
	this->fileMenu->addAction(this->loadFileAction);
	this->fileMenu->addSeparator();
	this->fileMenu->addAction(this->closeApplicationAction);

	this->settingsMenu = this->menuBar()->addMenu(tr("&Settings"));
	this->settingsMenu->addAction(this->editSettingsAction);
}

//void MainWindow::_pollInteractionsWithWindow() {
//
//	// Poll for and process events
//	glfwPollEvents();
//}
//
//void MainWindow::setOnScrollCallbackTo(std::function<void(double, double)> callback) {
//
//	this->_callbackAdapter.setOnMouseWheelCallbackTo(
//		[callback](GLFWwindow* windowPtr, double offsetX, double offsetY)->void { callback(offsetX, offsetY); }
//	);
//}
//
//void MainWindow::setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback) {
//
//	this->_callbackAdapter.setOnKeyCallbackTo(callback);
//}
//


