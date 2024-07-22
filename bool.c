typedef enum
{
    FALSE = 0,
    TRUE = !FALSE
} bool;

static bool ServoRunning = FALSE;
/*
 优点：
    控制性强：你可以完全控制bool类型的定义和行为。例如，你可以决定TRUE和FALSE具体数值。
    兼容性：在某些非常旧的 C 编译器中，<stdbool.h>可能不被支持，自定义类型确保在所有环境中都能使用。
缺点：
    区别性差异：其他开发者可能不习惯这种方式，阅读代码时需要一些时间才能理解。
    重复代码：如果项目中需要定义多个bool类型，每次都需要重复定义。 一致性问题：如果项目有不同的定义bool类型，可能会导致多个定义的不一致。
*/