#ifndef SUBSCRIBE_H
#define SUBSCRIBE_H
#include <QObject>
#include <QSet>
#include <QTimer>
#include <nzmqt/nzmqt.hpp>

namespace feeds {

class Subscribe : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString socketUri READ socketUri WRITE setSocketUri NOTIFY socketUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State)

public:
    explicit Subscribe(QObject *parent = 0);
    ~Subscribe();

    enum State {
        Down = 0,
        Trying = 1,
        Up = 2,
    };

    QString socketUri() const
    {
        return m_socketUri;
    }

    QString debugName() const
    {
        return m_debugName;
    }

    State state() const
    {
        return m_state;
    }

    QString errorString() const
    {
        return m_errorString;
    }
/*
    int heartbeatInterval() const
    {
        return m_heartbeatInterval;
    }
*/
    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setSocketUri(QString uri)
    {
        if (m_socketUri == uri)
            return;

        m_socketUri = uri;
        emit socketUriChanged(uri);
    }

    void setDebugName(QString debugName)
    {
        if (m_debugName == debugName)
            return;

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }
/*
    void setHeartbeatInterval(int interval)
    {
        if (m_heartbeatInterval == interval)
            return;

        m_heartbeatInterval = interval;
        emit heartbeatIntervalChanged(interval);
    }
*/
    void setReady(bool ready)
    {
        if (m_ready == ready)
            return;

        m_ready = ready;
        emit readyChanged(ready);

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }

    void addSocketTopic(const QString &name);
    void removeSocketTopic(const QString &name);
    void clearSocketTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_ready;
    QString m_debugName;

    QSet<QString> m_socketTopics;   // the topics we are interested in
    QString m_socketUri;
    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_socket;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;

   /* QTimer     *m_heartbeatTimer;
    int         m_heartbeatInterval;
    int         m_heartbeatLiveness;
    int         m_heartbeatResetLiveness;
    */
    // more efficient to reuse a protobuf Messages
    //pb::Container m_socketRx;
    QByteArray *data;

private slots:
/*
    void heartbeatTimerTick();
    void resetHeartbeatLiveness();
    void resetHeartbeatTimer();
    void startHeartbeatTimer();
    void stopHeartbeatTimer();
*/
    bool startSocket();
    void stopSocket();

    void processSocketMessage(const QList<QByteArray> &messageList);
    void socketError(int errorNum, const QString& errorMsg);


    void fsmDown();
    void fsmDownStartEvent();
    void fsmTrying();
    void fsmTryingFullUpdateReceivedEvent();
    void fsmTryingStopEvent();
    void fsmUp();
    //void fsmUpHeartbeatTimeoutEvent();
    //void fsmUpHeartbeatTickEvent();
    void fsmUpAnyMsgReceivedEvent();
    void fsmUpStopEvent();


signals:
    void socketUriChanged(QString uri);
    void socketMessageReceived( QByteArray topic,  QByteArray data );
    void socketMessageReceived2();

                               //const pb::Container &rx);
    void debugNameChanged(QString debugName);
    void stateChanged(Subscribe::State state);
    void errorStringChanged(QString errorString);
   // void heartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownStart(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingFullUpdateReceived(QPrivateSignal);
    void fsmTryingStop(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    //void fsmUpHeartbeatTimeout(QPrivateSignal);
    //void fsmUpHeartbeatTick(QPrivateSignal);
    void fsmUpAnyMsgReceived(QPrivateSignal);
    void fsmUpStop(QPrivateSignal);
};
}; // namespace feeds
#endif //SUBSCRIBE_H
