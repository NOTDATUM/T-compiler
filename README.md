# T 표준 라이브러리

#### T 표준 라이브러리 레퍼런스에서는 T언어의 정확한 문법과 의미를 포함하고 있습니다. T언어는 아래 나열된 목차의 기능들을 제공하고 있습니다. 표준 라이브러리는 컴포넌트를 포함하고 있지 않습니다. 컴포넌트들은 패키지 라이브러리에서 확인하실 수 있습니다.

## 개요
- 소개
- 내장형
    - 자료형
        - Number type
        - String type
        - Boolean type
        - Array type
        - Function type
        - Execution type
        - Class type
    - 함수형
        - do(), next()
        - print(), input()
        - len(), max(), min(), sort(), sorted()
        - {type}(), type()
    - 상수
        - true, false, undefined
        - null
- 연산
    - Number type 연산
        - =, +, -, *, /, //, %
        - +=, -=, *=, /=, //=, %=
    - String type 연산
        - =, +, -, *
    - Boolean type 연산
        - =, <, >
        - ==, !=, <=, >=
        - and, or, not, nor
    - Array type 연산
        - 아직 기획하지 않음


#### 아래 문서는 위에서 설명한 개요의 내용을 상세히 설명하고 있습니다. 정확한 문법의 의미를 기재했습니다.

## 소개
###### 나중에 쓰자...

## 내장형
> T언어에서 기본적으로 지원하고 있는 자료형, 문법, 상수에 대한 문서입니다.
## 자료형

### Number Type
> 숫자형 자료형
- Int
    - 정수형 자료형

        ```go
        def num.int a;
        def num.int b = 3;
        a = 5;
        ```

- Float
    - 실수형 자료형

        ```go
        def num.float c;
        def num.float d = 1.23;
        a = 4.56;
        ```
### String Type
> 문자형 자료형
    go
    def str a;
    def str b = "String";
    a = "T-language";
    b[3] = k;

### Boolean Type
> 논리형 자료형
- true, false, undefined
    > 상수에서 자세히 기술했음
    
    ```go
    def bool a;
    def bool b = false;
    a = true;
    b = undefined;
    ```
### Array Type
> 문자열형 자료형
- List
    - 리스트 자료형

        ```go 
        def arr.list a;
        def arr.list b = [0, 1, 2, 3];
        a = [0, 3];
        b[3] = 2;
        ```

- Dictionary
    - 딕셔너리 자료형

        ```go
        def arr.dict c;
        def arr.dict d = {"T":"compiler", "Python":"Interpreter"};
        c = {"first":1, "second":2, "third":3};
        ```

- Set
    - 집합형 자료형

### Function Type
> 함수형 자료형
- Static Function
    - 정적 함수
        
        ```go
        def func a;
        def func b = {print(1);};
        a = {print(2);};
        b;
        ```
        #### 매개변수가 주어질 수 없으며 단순 변수로 취급되는 함수이다. 변수로 취급되기 때문에 정적 함수의 경우 수정 가능하다. return 값을 지정할 수 없지만 정상적으로 실행되었을 때 true, 오류나 실행이 완료되지 않을 때 false를 반환한다.
- Dynamic Function
    - 동적 함수
                    
        ```go
        def func c(number);
        def func d(number) = {
            return number + 1;
        };
        def num.int a = 3;
        print(d(a));
        ```
        #### 매개변수가 주어질 수 있으며 호출 가능한 함수로써 기능을 하는 함수이다. 재귀형태를 구현하거나 데이터 값이 동적인 경우에 사용하는 함수이다. 선언 이후에 수정이 불가능하다. return 값을 지정하여 return 값을 반환한다. return 값이 주어지지 않았을 경우 정상적으로 실행되었을 때 true, 오류나 실행이 완료되지 않을 때 false를 반환한다.
### Execution Type
> 실행형 자료형
#### 실행형 자료형은 내장함수에 속한 자료형들입니다. 각각의 내장함수들은 자료형화 되어있어 각자의 기능을 효율적으로 이용할 수 있도록 합니다.
- do()
    > 내장함수에서 자세히 기술했음
    ```go
    a = do(n){
        b = do(n){
            print(a.cnt())
            print(b.cnt())
        }
        a.stop()
        print(a.cnt())
    };
    ```
    ```js
    < json "do" : Excution Type >
    {
        cnt: {
            # loop counter
            return loop_count; 
        }
        stop: {
            # stop execution
            return undefined;
        }
    }
    ```
- next()
    > 내장함수에서 자세히 기술했음
    ```
    작성 중입니다.
    ```
    ```
    작성 중입니다.
    ```
### Class Type
> Python과 동일하게 작동합니다.

## 함수형
- do(), next()
> 실행함수
```js
do({Number Type > -2}, {Boolean Type}, {Function Type(static)}) {
    {Function Type}
};
next({Function Type(static)}) {
    {Function Type}
};
```
- print(), input()
> 입 • 출력 함수
```js
print({String Type});
a = input() 
```
```js 
< name: a, type: {String Type} >
```
- len(), max(), min(), sort(), sorted()
> 리스트 관련 내장 함수
>> 위 문법은 python을 따라 갑니다.
- {type}(), type()
> 자료형 변환 함수, 자료형 지시 함수
```
def num.float a = 3.00;
print(str(a))
```