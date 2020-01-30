#include <QtTest/QtTest>
#include <GroupsIncMvc/private/gmvc_p.h>

class tst_GMvc : public QObject
{
    Q_OBJECT
public:
    tst_GMvc() {}

private slots:
    void initTestCase();

};

void tst_GMvc::initTestCase()
{
}

QTEST_MAIN(tst_GMvc)

#include "tst_gmvc.moc"
