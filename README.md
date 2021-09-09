# MSDB

An efficient array storage engine for managing multi-dimensional arrays.
--
The MSDB software provides various compression options to make the array compact, and it also can fastly perform queries on them.
This library can be embedded any C++ projects.
It adapts Array Functional Language (AFL), which is widely used in many array databases, instead of SQL.

## Query
You can query an array with C++ API, which provides AFL like operators.

### Build operator
- Example query for building two-dimensional array.
```C++
msdb::Build(
    0, msdb::dummy::star1024x1024::arrName,
    {
        msdb::DefDimension("Y", 0, 1024, 128, 32),
        msdb::DefDimension("X", 0, 1024, 128, 32)
    },
    {
        msdb::DefAttribute("ATTR_1", msdb::core::eleType::CHAR)
    }
);
```

### Between operator
- Example query for range selection on two-dimensional array.
```C++
msdb::Between(
    msdb::Load(msdb::Array(ctx, msdb::dummy::star1024x1024::arrName)),
    msdb::Domain(msdb::Coordinate({ 100, 100 }), msdb::Coordinate({ 104, 104 }))
);
```

### Filter operator
 - Example query for filter out value on two-dimensional array
 ```C++
 msdb::Filter(
    msdb::Load(msdb::Array(ctx, msdb::dummy::star1024x1024::arrName)),
    msdb::Attribute("ATTR_1") >= 7
);
 ```
