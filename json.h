#pragma once
#include "rapidjson/document.h" 
#include <cstdio>

using namespace rapidjson;

class Json {
public:
	Json() {}
	~Json() {}

	bool read(std::string const& file);
	bool write(std::string const& file);

	Document& document() { return document_; }

	Value makeStringValue(string str);

private:
	FILE* fp_ = nullptr;
	Document document_;
};

/*
�⺻���� ����

const char* json �̶�� json�����͸� ������ ������ �����
���⿡ 
{
	"hello", "world",
	"t" : true,
	"f" : false,
	"n" : null,
	"i" : 123,
	"pi" : 3.1416
	"a" : [1,2,3,4]
}

�̷��� ���ڿ��� �����ߴٰ� ġ��.

�׷���~

Document document; �̷��� ������ �ϳ� �����

document.Parse(json); �̶�� ġ��
�� document �ȿ� DOMƮ���� json�� �Ľ̵ȴ�!

��Ʈ���� ��� �Ǿ�������?
 http://postgame.tistory.com/m/558 ����
(���� ������Ʈ�� �迭�� ��Ʈ ���� ���ȴ�.)

�� �׷� �� DOMƮ���� ��� ����ұ�???

document.HasMember("hello");
document["hello"].IsString();
printf("hello = %s \n", document["hello"].GetString());

�̷��� ���� hollo = world �̷��� ���´�.

1. document�� ��Ʈ�� �ڽĳ�� �߿� "hello"��� ���� �����ϴ����� ���� ���Ǵ�.
2. document�� �ڽĳ�� �� helloŰ������ �˻��ؼ� ������ ���� string�������� ���´�.
3. document �ʿ��� hello�� �˻��ؼ� ���� Ű���� string���·� ĳ�����ؼ� ��ȯ�Ѵ�.

document["t"].IsBool();
bool t = (document["t"].GetBool())? "true" : "false";

bool���´� GetBool�� �������� �� ���ڿ��� "true" �Ǵ� "false"�� ������ ������
���� 3�� ������ ���� ���� �ɷ�����Ѵ�.

bool isNull = (document["n"].IsNull() ? "null" : "?")
null�� ����������.

json���� Ÿ���� ��� ���� ������ ��Ÿ����. �׷��� C++�� ���� ������� Ÿ���� �ʿ��ϴ�.

document["i"].IsNumber();
document["i"].IsInt();
document["i"].GetInt(); �Ǵ� (int)ducument["i"];

document["pi"].IsNumber();
document["pi"].IsDouble();
document["pi"].GetDouble(); 

json �迭�� ����� ���ڸ� �����Ѵ�.
const Value& a = document["a"]; //���� ������ ���� ���۷��� ����� ���� ������.

document�� aŰ���� Value��� Ŭ������ �����ؿԴ�. (���縦 ���� �ʱ� ������ �� ����)

a.IsArray();
for( SizeType i = 0; i < a.Size(); i++) //size_t��� SizeType���
	printf("a[%d] = %d \n", i, a[i].GetInt());

a�� �ڽ� ������ []�����ڿ� �ε��� ���� �־ �����ϸ鼭 int������ ĳ�����ؼ� �����´�.
�⺻������ SizeType�� unsigned�� typedef��. (�׷� �׳� size_t ����;;)
�迭�� std::vector�� �����ϴ�. (�ݺ��ڷ� ��ȸ�� ����)


Query Object
�迭�� �����ϰ�, ���ͷ����ͷ� ������Ʈ ����� �ݺ��� �� �ִ�.

static const char* kTypeNames[] = {"Null", "False", "True", "Object", "Array", "String", "Number"};

for(Value::ConstMemberIterator itr = document.MemberBegin();
    itr != document.MemberEnd(); ++itr)
	{
		printf(" Type of member %s is %s \n",
				itr->name.GetString(), kTypeNames[itr->value.GetType()]);
	}

Type of member hello is String
Type of member t is True
...
...

DOMƮ�� ���� �� ���� �����ߴٸ� �ٽ� Writer�� ����ؼ� JSON���� ���� �� �� �ִ�.

����Ʈ �����ڷ� ���̳� ��ť��Ʈ�� ���� �� Ÿ���� Null�̴�.
Ÿ���� ������ ���� SetXXX()�� ȣ���ϰų� �Ҵ������ڸ� ����Ѵ�.

Document d;  //Null
d.SetObject();

Value v; //Null
v.SetInt(10); �Ǵ� v = 10;

������ �����ε�
Value b(true);
Value i(-123);
Value u(123u);
Value d(1.5); //������

�� ������Ʈ�� �迭�� �����ϱ� ���ؼ��� ����Ʈ ������ ���� SetObject()�� SetArray()�� ����ϰų�
�� ���� ������ Value(Type)�� ����ϸ� �ȴ�.

Value o(kObjectType);
Value a(kArrayType);

������

rapidJson�� ���� �������� �ʴ´�. �׻� �̵���Ų��.

Value a(123);
Value b(456);

b = a; //a�� ���� b�� �̵��ϰ� a�� Null�� �ȴ�. (������� �ʴ´�.)


�迭�� �� �߰�
Value a(kArrayType);
Document::AllocatorType& allocator = document.GetAllocator();

a.PushBack(Value(42), allocator); //�̰� �������� �ʴ´�.
a.PushBack(Value().SetInt(42), allocator); �̷��� �ؾߵȴ�.
a.PushBack(Value(42).Move(), allocator); ���� ����.


���ڿ� ����
1. copy-string : �� ���ۿ� �Ҵ��ϰ� �ҽ� �����͸� �� ���� ī��
2. const-string: ������ ���ڿ��� �����͸� �����Ѵ�.

Document document;
Value author;
char buffer[10];
int len = sprintf(buffer, "%s %s", "Milo", "Yip");

//���ڿ� ���� ����
author.SetString(buffer, len, document.GetAllocator());

Document �ν��Ͻ��κ��� �Ҵ��ڸ� ��´�.

������ ���
Value s;
s.SetString("rapidjson");
s = "rapidjson"; ���� ����.

//�迭�� ����
�迭�� ��Ҹ� �Ҵ��Ϸ��� �Ҵ��ڰ� �ʿ��ϴ�.
PushBack �� �ʿ�! �Ҵ��ڴ� document.GetAllocator()�� ������ �ȴ�.

//������Ʈ�� ����
������Ʈ�� key-value ����� �����̴�.
�� Ű�� �ݵ�� ���ڿ��̾�� �Ѵ�. 
������Ʈ�� �����ϴ� ����� ����� �߰��ϴ� ���̴�.

Value& AddMember(Value&, Value&, Allocator& allocator)
Value& AddMember(StringRefType, Value&, Allocator&)

Value contact(kObject);
contact.AddMember("name", "Milo", document.GetAllocator());
contact.AddMember("married", true, document.GetAllocator());

����� �̵�?
RemoveMember(name);
EraseMember(���ͷ�����)
EraseMember(���ͷ����� first, ���ͷ����� last);

//���� ����(�Ҵ��ڰ� �ʿ�)
Document d;
Document::AllocatorType& a = d.GetAllocator();

Value v1("foo");
Value v2(v1, a);

��ť��Ʈ ����
Value v2();
v2.CopyFrom(d, a);

���� swap
Value a(1234);
Value b("Hello");
a.Swap(b); 





*/