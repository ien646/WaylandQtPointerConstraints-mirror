#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#include <WaylandQtPointerConstraints/LockPointer.h>

class ConfiningButton final : public QPushButton
{
public:
    explicit ConfiningButton(QWidget* parentWidget)
        : QPushButton(parentWidget)
    {
        setText("Freeze!");

    }

private:
    std::unique_ptr<LockPointer> _lockPointer;
    QWidget* _nativeWidget = nullptr;

    void mousePressEvent(QMouseEvent* event) override
    {
        if (!_nativeWidget)
        {
            if (windowHandle())
            {
                _nativeWidget = this;
            }
            else if (nativeParentWidget())
            {
                _nativeWidget = nativeParentWidget();
            }
            else
            {
                // No native parent found
                exit(-1);
            }
        }

        if (!_lockPointer)
        {
            _lockPointer = std::make_unique<LockPointer>(_nativeWidget->windowHandle());
        }

        if (!_lockPointer->isPointerLocked())
        {
            const QPoint lockPos = mapTo(_nativeWidget, QPoint{0, 0});
            _lockPointer->lockPointer(QRect{lockPos, QSize{width(), height()}});
        }
        else
        {
            _lockPointer->unlockPointer();
        }
    }
};

class MainWindow final : public QMainWindow
{
public:
    MainWindow()
    {
        const auto centralWidget = new QWidget(this);
        const auto layout = new QHBoxLayout(this);
        layout->addWidget(new QLabel("Not here"));
        layout->addWidget(new ConfiningButton(this));
        layout->addWidget(new QLabel("Not here"));
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
    }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWindow win;
    win.show();

    return app.exec();
}
