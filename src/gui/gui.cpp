#include "gui.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

MainWindow::MainWindow() : currentDirectory(homeDirectory)
{
    setWindowTitle("CleanupPro");
    resize(900, 640);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(16);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(2);

    titleLabel = new QLabel("CleanupPro");
    titleLabel->setObjectName("title");

    QLabel* subtitleLabel =
        new QLabel("Review and remove files taking up space.");
    subtitleLabel->setObjectName("subtitle");

    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subtitleLabel);

    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    backButton = new QPushButton("Back");
    backButton->setObjectName("backButton");
    backButton->setCursor(Qt::PointingHandCursor);

    scanButton = new QPushButton("Refresh");
    scanButton->setObjectName("refreshButton");
    scanButton->setCursor(Qt::PointingHandCursor);

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(scanButton);

    layout->addLayout(headerLayout);

    directoryLabel = new QLabel(
        QString::fromStdString(currentDirectory.string())
    );
    directoryLabel->setObjectName("directory");

    layout->addWidget(directoryLabel);

    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("separator");

    layout->addWidget(separator);

    fileList = new QListWidget();
    fileList->setObjectName("fileList");
    fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileList->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(fileList, 1);

    QHBoxLayout* bottomLayout = new QHBoxLayout();

    QLabel* helpLabel =
        new QLabel("Double-click a folder to open it.");
    helpLabel->setObjectName("helpText");

    deleteButton = new QPushButton("Delete Selected");
    deleteButton->setObjectName("deleteButton");
    deleteButton->setCursor(Qt::PointingHandCursor);

    bottomLayout->addWidget(helpLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(deleteButton);

    layout->addLayout(bottomLayout);

    connect(scanButton, &QPushButton::clicked, this, [this]() {
        scanDirectory();
    });

    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        deleteSelected();
    });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        if (currentDirectory == homeDirectory) {
            return;
        }

        currentDirectory = currentDirectory.parent_path();
        scanDirectory();
    });

    connect(
        fileList,
        &QListWidget::itemDoubleClicked,
        this,
        [this](QListWidgetItem* item) {
            int row = fileList->row(item);

            if (row < 0 ||
                static_cast<size_t>(row) >= currentEntries.size()) {
                return;
            }

            const Entry& entry = currentEntries[row];

            if (!entry.isDirectory) {
                return;
            }

            currentDirectory = entry.path;
            scanDirectory();
        }
    );

    QTimer::singleShot(0, this, [this]() {
        scanDirectory();
    });
}

void MainWindow::scanDirectory()
{
    directoryLabel->setText(
        QString::fromStdString(currentDirectory.string())
    );

    backButton->setEnabled(currentDirectory != homeDirectory);

    fileList->clear();
    fileList->addItem("Scanning...");

    scanButton->setText("Scanning...");
    scanButton->setEnabled(false);
    deleteButton->setEnabled(false);

    auto* watcher =
        new QFutureWatcher<std::vector<Entry>>(this);

    connect(
        watcher,
        &QFutureWatcher<std::vector<Entry>>::finished,
        this,
        [this, watcher]() {
            currentEntries = watcher->result();

            std::sort(
                currentEntries.begin(),
                currentEntries.end(),
                [](const Entry& a, const Entry& b) {
                    return a.size > b.size;
                }
            );

            displayEntries();

            scanButton->setText("Refresh");
            scanButton->setEnabled(true);
            deleteButton->setEnabled(true);

            watcher->deleteLater();
        }
    );

    fs::path directory = currentDirectory;

    watcher->setFuture(
        QtConcurrent::run([directory]() {
            return checkFolder(directory);
        })
    );
}

void MainWindow::displayEntries()
{
    fileList->clear();

    for (const auto& entry : currentEntries) {
        std::string name = entry.path.filename().string();
        std::string size = formatSize(entry.size);
        std::string type = entry.isDirectory ? "Folder" : "File";

        std::string text =
            name + "    ·    " + size + "    ·    " + type;

        fileList->addItem(QString::fromStdString(text));
    }
}

void MainWindow::deleteSelected()
{
    QList<QListWidgetItem*> selectedItems =
        fileList->selectedItems();

    if (selectedItems.empty()) {
        QMessageBox::information(
            this,
            "No Selection",
            "Select a file or folder first."
        );
        return;
    }

    QString message;

    if (selectedItems.size() == 1) {
        message = "Are you sure you want to delete this item?";
    } else {
        message =
            "Are you sure you want to delete these " +
            QString::number(selectedItems.size()) +
            " items?";
    }

    QMessageBox::StandardButton answer =
        QMessageBox::question(
            this,
            "Delete Items",
            message,
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

    if (answer != QMessageBox::Yes) {
        return;
    }

    for (QListWidgetItem* item : selectedItems) {
        int row = fileList->row(item);

        if (row < 0 ||
            static_cast<size_t>(row) >= currentEntries.size()) {
            continue;
        }

        fs::path selectedPath = currentEntries[row].path;

        try {
            deleteFile(selectedPath);
        }
        catch (const std::exception& e) {
            QMessageBox::warning(
                this,
                "Delete Failed",
                QString::fromStdString(e.what())
            );
        }
    }

    scanDirectory();
}