#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollBar>
#include <QProcess>

void addMessage(QWidget *container, const QString &message, Qt::Alignment alignment, bool isUser) {
    QTextEdit *chatHistory = qobject_cast<QTextEdit*>(container);
    if (!chatHistory) {
        return;
    }

    // Set HTML formatting for the message
    QString formattedMessage;
    if (isUser) {
        formattedMessage = "<div style=\"text-align: right; color: #FF0000;\"><b>  You:</b> " + message + "</div>";
    } else {
        formattedMessage = "<div style=\"text-align: left; color: #ADD8E6;\"><b>  Celia:</b> " + message + "</div>";
    }

    // Append the formatted message to the chat history
    chatHistory->append(formattedMessage);

    // Scroll to the bottom
    chatHistory->verticalScrollBar()->setValue(chatHistory->verticalScrollBar()->maximum());
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create main window
    QWidget window;
    window.setWindowTitle("Celia Personal Assistant");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    // Chat history display
    QTextEdit *chatHistory = new QTextEdit(&window);
    chatHistory->setReadOnly(true);
    chatHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); // Enable vertical scrolling
    mainLayout->addWidget(chatHistory);

    // Input box
    QLineEdit *inputBox = new QLineEdit(&window);
    mainLayout->addWidget(inputBox);

    // Button
    QPushButton *sendButton = new QPushButton("Send", &window);
    mainLayout->addWidget(sendButton);

    // Start the Python script to get the welcome message
    QProcess pythonProcessInit;
    pythonProcessInit.start("python3", QStringList() << "PythonFiles/welcome.py");
    pythonProcessInit.waitForStarted();
    pythonProcessInit.waitForFinished();
    QString starterMessage = pythonProcessInit.readAllStandardOutput().trimmed();
    addMessage(chatHistory, starterMessage, Qt::AlignLeft, false); // AI message

    // Connect button click event to add user message and retrieve AI response
    QObject::connect(sendButton, &QPushButton::clicked, [&]() {
        QString userInput = inputBox->text();
        if (!userInput.isEmpty()) {

            chatHistory->append("<br>");

            // Add user message
            addMessage(chatHistory, userInput, Qt::AlignRight, true);

            chatHistory->append("<br>");

            // Clear input box
            inputBox->clear();

            // Start Python script to get AI response
            QProcess pythonProcessFollow;
            pythonProcessFollow.start("python3", QStringList() << "PythonFiles/furtherInquire.py" << userInput);
            pythonProcessFollow.waitForStarted();
            pythonProcessFollow.waitForFinished();
            QString followUpMessage = pythonProcessFollow.readAllStandardOutput().trimmed();

            // Add AI response message
            addMessage(chatHistory, followUpMessage, Qt::AlignLeft, false);
        }
    });

    window.show();
    return app.exec();
}
