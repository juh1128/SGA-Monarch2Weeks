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
기본적인 사용법

const char* json 이라고 json데이터를 저장할 공간을 만들고
여기에 
{
	"hello", "world",
	"t" : true,
	"f" : false,
	"n" : null,
	"i" : 123,
	"pi" : 3.1416
	"a" : [1,2,3,4]
}

이렇게 문자열을 저장했다고 치자.

그러면~

Document document; 이렇게 문서를 하나 만들고

document.Parse(json); 이라고 치면
이 document 안에 DOM트리로 json이 파싱된다!

돔트리가 어떻게 되어있을까?
 http://postgame.tistory.com/m/558 참고
(오직 오브젝트나 배열만 루트 값을 허용된다.)

자 그럼 이 DOM트리를 어떻게 사용할까???

document.HasMember("hello");
document["hello"].IsString();
printf("hello = %s \n", document["hello"].GetString());

이렇게 쓰면 hollo = world 이렇게 나온다.

1. document의 루트의 자식노드 중에 "hello"라는 값이 존재하는지를 묻는 질의다.
2. document의 자식노드 중 hello키값으로 검색해서 나오는 값이 string형인지를 묻는다.
3. document 맵에서 hello로 검색해서 나온 키값을 string형태로 캐스팅해서 반환한다.

document["t"].IsBool();
bool t = (document["t"].GetBool())? "true" : "false";

bool형태는 GetBool로 가져왔을 때 문자열로 "true" 또는 "false"로 나오기 때문에
저런 3항 연산자 등을 통해 걸러줘야한다.

bool isNull = (document["n"].IsNull() ? "null" : "?")
null도 마찬가지다.

json숫자 타입은 모두 숫자 값으로 나타난다. 그러나 C++은 보다 명시적인 타입이 필요하다.

document["i"].IsNumber();
document["i"].IsInt();
document["i"].GetInt(); 또는 (int)ducument["i"];

document["pi"].IsNumber();
document["pi"].IsDouble();
document["pi"].GetDouble(); 

json 배열은 요소의 숫자를 포함한다.
const Value& a = document["a"]; //연속 참조에 대한 레퍼런스 사용은 쉽고 빠르다.

document의 a키값을 Value라는 클래스에 참조해왔다. (복사를 하지 않기 때문에 더 빠름)

a.IsArray();
for( SizeType i = 0; i < a.Size(); i++) //size_t대신 SizeType사용
	printf("a[%d] = %d \n", i, a[i].GetInt());

a의 자식 노드들을 []연산자에 인덱스 값을 넣어서 접근하면서 int형으로 캐스팅해서 가져온다.
기본적으로 SizeType은 unsigned의 typedef다. (그럼 그냥 size_t 쓰자;;)
배열은 std::vector와 유사하다. (반복자로 순회도 가능)


Query Object
배열과 유사하게, 이터레이터로 오브젝트 멤버를 반복할 수 있다.

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

DOM트리 생성 후 값을 조작했다면 다시 Writer를 사용해서 JSON으로 저장 할 수 있다.

디폴트 생성자로 값이나 도큐먼트를 만들 때 타입은 Null이다.
타입을 변경할 때는 SetXXX()를 호출하거나 할당지정자를 사용한다.

Document d;  //Null
d.SetObject();

Value v; //Null
v.SetInt(10); 또는 v = 10;

생성자 오버로드
Value b(true);
Value i(-123);
Value u(123u);
Value d(1.5); //더블형

빈 오브젝트나 배열을 생성하기 위해서는 디폴트 생성자 이후 SetObject()나 SetArray()를 사용하거나
한 눈에 들어오게 Value(Type)을 사용하면 된다.

Value o(kObjectType);
Value a(kArrayType);

유의점

rapidJson은 값을 복사하지 않는다. 항상 이동시킨다.

Value a(123);
Value b(456);

b = a; //a의 값이 b로 이동하고 a는 Null이 된다. (복사되지 않는다.)


배열에 값 추가
Value a(kArrayType);
Document::AllocatorType& allocator = document.GetAllocator();

a.PushBack(Value(42), allocator); //이건 동작하지 않는다.
a.PushBack(Value().SetInt(42), allocator); 이렇게 해야된다.
a.PushBack(Value(42).Move(), allocator); 위와 같다.


문자열 저장
1. copy-string : 한 버퍼에 할당하고 소스 데이터를 그 곳에 카피
2. const-string: 간단히 문자열의 포인터를 저장한다.

Document document;
Value author;
char buffer[10];
int len = sprintf(buffer, "%s %s", "Milo", "Yip");

//문자열 동적 생성
author.SetString(buffer, len, document.GetAllocator());

Document 인스턴스로부터 할당자를 얻는다.

간단한 방법
Value s;
s.SetString("rapidjson");
s = "rapidjson"; 위와 같다.

//배열의 수정
배열의 요소를 할당하려면 할당자가 필요하다.
PushBack 시 필요! 할당자는 document.GetAllocator()로 얻어오면 된다.

//오브젝트의 수정
오브젝트는 key-value 페어의 모음이다.
각 키는 반드시 문자열이어야 한다. 
오브젝트를 조작하는 방법은 멤버를 추가하는 것이다.

Value& AddMember(Value&, Value&, Allocator& allocator)
Value& AddMember(StringRefType, Value&, Allocator&)

Value contact(kObject);
contact.AddMember("name", "Milo", document.GetAllocator());
contact.AddMember("married", true, document.GetAllocator());

멤버의 이동?
RemoveMember(name);
EraseMember(이터레이터)
EraseMember(이터레이터 first, 이터레이터 last);

//값의 복사(할당자가 필요)
Document d;
Document::AllocatorType& a = d.GetAllocator();

Value v1("foo");
Value v2(v1, a);

도큐먼트 복사
Value v2();
v2.CopyFrom(d, a);

값의 swap
Value a(1234);
Value b("Hello");
a.Swap(b); 





*/