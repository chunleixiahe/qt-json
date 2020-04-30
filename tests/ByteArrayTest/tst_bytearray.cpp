#include <QtTest>
#include <serializablebytearray.h>
#include <serializationtest.h>
using namespace QtJson;

class ByteArrayTest : public SerializationTest<SerializableByteArray>
{
    Q_OBJECT

protected:
    void setupData() const override;
    void setupDeserData() const override;

private:
    static inline SerializableByteArray d(const QByteArray &d) {
        return d;
    }
    static inline CommonConfiguration c(ByteArrayMode mode) {
        CommonConfiguration config;
        config.byteArrayMode = mode;
        return config;
    }
};

void ByteArrayTest::setupData() const
{
    QTest::addRow("base64") << c(ByteArrayMode::Base64)
                            << d("test")
                            << QJsonValue{QStringLiteral("dGVzdA==")}
                            << QCborValue{QCborKnownTags::ExpectedBase64, QByteArray{"test"}}
                            << false;
    QTest::addRow("base64url") << c(ByteArrayMode::Base64url)
                               << d("test")
                               << QJsonValue{QStringLiteral("dGVzdA")}
                               << QCborValue{QCborKnownTags::ExpectedBase64url, QByteArray{"test"}}
                               << false;
    QTest::addRow("hex") << c(ByteArrayMode::Hex)
                         << d("test")
                         << QJsonValue{QStringLiteral("74657374")}
                         << QCborValue{QCborKnownTags::ExpectedBase16, QByteArray{"test"}}
                         << false;
}

void ByteArrayTest::setupDeserData() const
{
    QTest::addRow("cbor.untagged") << c(ByteArrayMode::Base64)
                                   << d("test")
                                   << QJsonValue{QJsonValue::Undefined}
                                   << QCborValue{QByteArray{"test"}}
                                   << false;
}

QTEST_APPLESS_MAIN(ByteArrayTest)

#include "tst_bytearray.moc"
