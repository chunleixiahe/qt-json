#include <QtTest>
#include <serializationtest.h>
#include <serializablearray.h>
#include <testserializable.h>
using namespace QtJson;

class ArrayTest : public SerializationMultiTest<
                      QList<int>,
                      QVector<int>,
#ifndef QT_NO_LINKED_LIST
                      QLinkedList<int>,
#endif
                      QQueue<int>,
                      QStack<int>,
                      QSet<int>,
                      QList<TestSerializable>>
{
	Q_OBJECT

protected:
	void setupData() const override;
	void setupSerData() const override;
	void setupDeserData() const override;

private:
	template <typename... TArgs>
    inline Variant dl(TArgs&&... args) const {
        return d<QList<int>>(std::forward<TArgs>(args)...);
    }
    template <typename... TArgs>
    inline Variant dv(TArgs&&... args) const {
        return d<QVector<int>>(std::forward<TArgs>(args)...);
    }
#ifndef QT_NO_LINKED_LIST
    template <typename... TArgs>
    inline Variant dll(TArgs&&... args) const {
        return d<QLinkedList<int>>(std::forward<TArgs>(args)...);
    }
#endif
	template <typename... TArgs>
    inline Variant dq(TArgs&&... args) const {
        QQueue<int> q;
        int x[] = {0, (q.append(args),0)...};
        Q_UNUSED(x);
        return d<QQueue<int>>(std::move(q));
	}
	template <typename... TArgs>
    inline Variant ds(TArgs&&... args) const {
        QStack<int> s;
        int x[] = {0, (s.append(args),0)...};
        Q_UNUSED(x);
        return d<QStack<int>>(std::move(s));
	}
	template <typename... TArgs>
    inline Variant dhs(TArgs&&... args) const {
        return d<QSet<int>>(std::forward<TArgs>(args)...);
	}
	template <typename... TArgs>
    inline Variant dls(TArgs&&... args) const {
        return d<QList<TestSerializable>>(std::forward<TArgs>(args)...);
	}
};

Q_DECLARE_METATYPE(ArrayTest::Variant)

void ArrayTest::setupData() const
{
	QTest::addRow("list.filled") << Configuration{}
								 << dl(1, 2, 3)
								 << QJsonValue{QJsonArray{1, 2, 3}}
								 << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
								 << false;
	QTest::addRow("list.empty") << Configuration{}
								<< dl()
								<< QJsonValue{QJsonArray{}}
								<< QCborValue{HomogeneousArrayTag, QCborArray{}}
								<< false;

	QTest::addRow("vector.filled") << Configuration{}
								   << dv(1, 2, 3)
								   << QJsonValue{QJsonArray{1, 2, 3}}
								   << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
								   << false;
	QTest::addRow("vector.empty") << Configuration{}
								  << dv()
								  << QJsonValue{QJsonArray{}}
								  << QCborValue{HomogeneousArrayTag, QCborArray{}}
								  << false;

#ifndef QT_NO_LINKED_LIST
	QTest::addRow("linkedlist.filled") << Configuration{}
									   << dll(1, 2, 3)
									   << QJsonValue{QJsonArray{1, 2, 3}}
									   << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
									   << false;
	QTest::addRow("linkedlist.empty") << Configuration{}
									  << dll()
									  << QJsonValue{QJsonArray{}}
									  << QCborValue{HomogeneousArrayTag, QCborArray{}}
									  << false;
#endif

	QTest::addRow("queue.filled") << Configuration{}
								  << dq(1, 2, 3)
								  << QJsonValue{QJsonArray{1, 2, 3}}
								  << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
								  << false;
	QTest::addRow("queue.empty") << Configuration{}
								 << dq()
								 << QJsonValue{QJsonArray{}}
								 << QCborValue{HomogeneousArrayTag, QCborArray{}}
								 << false;

	QTest::addRow("stack.filled") << Configuration{}
								  << ds(1, 2, 3)
								  << QJsonValue{QJsonArray{1, 2, 3}}
								  << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
								  << false;
	QTest::addRow("stack.empty") << Configuration{}
								 << ds()
								 << QJsonValue{QJsonArray{}}
								 << QCborValue{HomogeneousArrayTag, QCborArray{}}
								 << false;

	QTest::addRow("set.empty") << Configuration{}
							   << dhs()
							   << QJsonValue{QJsonArray{}}
							   << QCborValue{FiniteSetTag, QCborArray{}}
							   << false;

	QTest::addRow("list.serializable.filled") << Configuration{}
											  << dls(1.1, 2.2, 3.3)
											  << QJsonValue{QJsonArray{1.1, 2.2, 3.3}}
											  << QCborValue{HomogeneousArrayTag, QCborArray{1.1, 2.2, 3.3}}
											  << false;
	QTest::addRow("list.serializable.empty") << Configuration{}
											 << dls()
											 << QJsonValue{QJsonArray{}}
											 << QCborValue{HomogeneousArrayTag, QCborArray{}}
											 << false;
}

void ArrayTest::setupSerData() const
{
	QTest::addRow("set.filled") << Configuration{}
								<< dhs(1)
								<< QJsonValue{QJsonArray{1}}
								<< QCborValue{FiniteSetTag, QCborArray{1}}
								<< false;
}

void ArrayTest::setupDeserData() const
{
	QTest::addRow("set.filled") << Configuration{}
								<< dhs(1, 2, 3)
								<< QJsonValue{QJsonArray{1, 2, 3}}
								<< QCborValue{FiniteSetTag, QCborArray{1, 2, 3}}
								<< false;

	QTest::addRow("list.untagged") << Configuration{}
								   << dl(1, 2, 3)
								   << QJsonValue{QJsonValue::Undefined}
								   << QCborValue{QCborArray{1, 2, 3}}
								   << false;

	QTest::addRow("vector.untagged") << Configuration{}
									 << dv(1, 2, 3)
									 << QJsonValue{QJsonValue::Undefined}
									 << QCborValue{QCborArray{1, 2, 3}}
									 << false;

#ifndef QT_NO_LINKED_LIST
	QTest::addRow("linkedlist.untagged") << Configuration{}
										 << dll(1, 2, 3)
										 << QJsonValue{QJsonArray{1, 2, 3}}
										 << QCborValue{QCborArray{1, 2, 3}}
										 << false;
#endif

	QTest::addRow("queue.untagged") << Configuration{}
									<< dq(1, 2, 3)
									<< QJsonValue{QJsonValue::Undefined}
									<< QCborValue{QCborArray{1, 2, 3}}
									<< false;

	QTest::addRow("stack.untagged") << Configuration{}
									<< ds(1, 2, 3)
									<< QJsonValue{QJsonValue::Undefined}
									<< QCborValue{QCborArray{1, 2, 3}}
									<< false;

	QTest::addRow("set.untagged") << Configuration{}
								  << dhs()
								  << QJsonValue{QJsonValue::Undefined}
								  << QCborValue{QCborArray{1, 2, 3}}
								  << true;

	QTest::addRow("list.serializable.untagged") << Configuration{}
												<< dls(1.1, 2.2, 3.3)
												<< QJsonValue{QJsonValue::Undefined}
												<< QCborValue{QCborArray{1.1, 2.2, 3.3}}
												<< false;

	QTest::addRow("list.invalid") << Configuration{}
								  << dl()
								  << QJsonValue{QJsonValue::Undefined}
								  << QCborValue{42}
								  << true;
	QTest::addRow("set.invalid") << Configuration{}
								  << dhs()
								  << QJsonValue{QJsonValue::Undefined}
								  << QCborValue{42}
								  << true;
}

QTEST_APPLESS_MAIN(ArrayTest)

#include "tst_array.moc"
