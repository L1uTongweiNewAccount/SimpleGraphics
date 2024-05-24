const fs = require('node:fs');

var Symbols = fs.openSync("build/include/symbols.h", 'w');
var ctags_file = fs.readFileSync("build/target/SimpleGraphics.symbols.jsonl").toString().split("\n");
var map = fs.readFileSync("build/target/SimpleGraphics.map").toString().split("\n");
var ctags = new Array;
for(var i = 0; i < ctags_file.length; i++){
    if(ctags_file[i] == "") continue;
    ctags.push(JSON.parse(ctags_file[i]));
}

var MapStart = 0, MapEnd = 0;
for(var i = 0; i < map.length; i++){
    if(map[i] == "Linker script and memory map"){
        MapStart = i + 2;
    }else if(map[i] == "Cross Reference Table"){
        MapEnd = i - 2;
    }
    map[i] = map[i].trim().replace(/\s+/g, " ");
}

fs.writeSync(Symbols, "#pragma once\n");
fs.writeSync(Symbols, "#include <stdint.h>\n");
fs.writeSync(Symbols, "#include <stdatomic.h>\n")
fs.writeSync(Symbols, "#define __I volatile const\n");
fs.writeSync(Symbols, "#define __O volatile\n");
fs.writeSync(Symbols, "#define __IO volatile\n");
fs.writeSync(Symbols, "/* Symbols map - Auto Generated, DO NOT MODIFY! */\n\n");

fs.writeSync(Symbols, "/* Macros */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "macro"){
        //#define MacroName MacroDef
        fs.writeSync(Symbols, "#define " + ctags[i]["name"] + " " + ctags[i]["macrodef"] + "\n");
        
    }
}
fs.writeSync(Symbols, "\n");

fs.writeSync(Symbols, "/* Typedefined Structures or Type */\n");
fs.writeSync(Symbols, "typedef void (*pFunc)(void);");
fs.writeSync(Symbols, "typedef void (*funcptr_void)(void);");
fs.writeSync(Symbols, "typedef void *osMemoryPoolId_t;");
fs.writeSync(Symbols, "typedef void *osMessageQueueId_t;");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "typedef"){
        if(ctags[i].typeref.substring(0, 8) == "typename"){
            //typedef TypeSrc TypeDst;
            //fs.writeSync(Symbols, "typedef " + ctags[i].typeref.substring(9) + " " + ctags[i].name + ";\n");
        }else{
            //typedef struct/enum {} xxx;
            var source = fs.readFileSync(ctags[i].path);
            var pattern = new RegExp("typedef(.*?)" + 
                ctags[i].pattern.substring(2, ctags[i].pattern.length - 2)
                .replace(/\*/g, "\\*")
                .replace(/\{/g, "\\{")
                .replace(/\}/g, "\\}")
                .replace(/\(/g, "\\(")
                .replace(/\)/g, "\\)")
                .replace(/\+/g, "\\+")
                .replace(/\//g, "\\/")
            , "gm");
            var final = pattern.exec(source.toString()
                .replace(/\s*\/\/.*$/mg, "")
                .replace(/\/\*(.|\r\n|\n)*?\*\//g, "")
                .replace(/(\n|\r|\t)/g, "")
            );
            if(final == null) continue;
            fs.writeSync(Symbols, final[0].replace(/;/g, ";\n"));
        }
    }
}
fs.writeSync(Symbols, "\n");

var getAddress = ((tab) => {
    for(var j = MapStart; j <= MapEnd; j++){
        var location = map[j].split(' ');
        if(location.length != 2) continue;
        if(location[1] == tab.name){
            return location[0];
        }
    }
    return null;
});

fs.writeSync(Symbols, "/* Functions */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "function"){
        //#define FunctionName ((ReturnType(*)(Arguments))(void*)address)
        var address = getAddress(ctags[i]);
        if(address == null || /weak/.test(ctags[i].pattern) || ctags[i].name == "main"){
            //console.warn("Warning: Cannot found symbol " + ctags[i].name + " in map file.");
            continue;
        }
        fs.writeSync(Symbols, "#define " + ctags[i].name + " ");
        fs.writeSync(Symbols, "((" + ctags[i].typeref.substring(9) + "(*)" + ((tab) => {
            var source = fs.readFileSync(tab.path);
            var pattern = new RegExp(tab.typeref.substring(9).replace(/\*/g, "\\*") + "(\\s*)" + 
                tab.name + "(\\s*)(.+?\\))", "gm");
            var final = pattern.exec(source.toString().replace(/(\n|\r|\t)/g, ""));
            //assert(final.length > 2);
            return final[2];
        })(ctags[i]) + ")(void*)" + address + ")\n");
    }
}
fs.writeSync(Symbols, "\n");

fs.writeSync(Symbols, "/* Global Variables */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern file kind (typeref, macrodef)
    if(ctags[i].kind == "variable"){
        //#define VariablePointer ((Type*)(void*)address)
        var address = getAddress(ctags[i]);
        if(address == null){
            //console.log("Warning: Cannot found symbol " + ctags[i].name + " in map file.");
            continue;
        }
        fs.writeSync(Symbols, "#define " + ctags[i].name + " ");
        fs.writeSync(Symbols, "((" + ctags[i].typeref.substring(9) + "*)(void*)" + address + ")\n");
    }
}const { assert } = require('console');

var Symbols = fs.openSync("build/include/symbols.h", 'w');
var ctags_file = fs.readFileSync("build/target/SimpleGraphics.symbols.jsonl").toString().split("\n");
var map = fs.readFileSync("build/target/SimpleGraphics.map").toString().split("\n");
var ctags = new Array;
for(var i = 0; i < ctags_file.length; i++){
    if(ctags_file[i] == "") continue;
    ctags.push(JSON.parse(ctags_file[i]));
}

var MapStart = 0, MapEnd = 0;
for(var i = 0; i < map.length; i++){
    if(map[i] == "Linker script and memory map"){
        MapStart = i + 2;
    }else if(map[i] == "Cross Reference Table"){
        MapEnd = i - 2;
    }
    map[i] = map[i].trim().replace(/\s+/g, " ");
}

fs.writeSync(Symbols, "#pragma once\n");
fs.writeSync(Symbols, "#include <stdint.h>\n");
fs.writeSync(Symbols, "#include <stdatomic.h>\n")
fs.writeSync(Symbols, "#define __I volatile const\n");
fs.writeSync(Symbols, "#define __O volatile\n");
fs.writeSync(Symbols, "#define __IO volatile\n");
fs.writeSync(Symbols, "/* Symbols map - Auto Generated, DO NOT MODIFY! */\n\n");

fs.writeSync(Symbols, "/* Macros */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "macro"){
        //#define MacroName MacroDef
        fs.writeSync(Symbols, "#define " + ctags[i]["name"] + " " + ctags[i]["macrodef"] + "\n");
        
    }
}
fs.writeSync(Symbols, "\n");

fs.writeSync(Symbols, "/* Typedefined Structures or Type */\n");
fs.writeSync(Symbols, "typedef void (*pFunc)(void);");
fs.writeSync(Symbols, "typedef void (*funcptr_void)(void);");
fs.writeSync(Symbols, "typedef void *osMemoryPoolId_t;");
fs.writeSync(Symbols, "typedef void *osMessageQueueId_t;");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "typedef"){
        if(ctags[i].typeref.substring(0, 8) == "typename"){
            //typedef TypeSrc TypeDst;
            //fs.writeSync(Symbols, "typedef " + ctags[i].typeref.substring(9) + " " + ctags[i].name + ";\n");
        }else{
            //typedef struct/enum {} xxx;
            var source = fs.readFileSync(ctags[i].path);
            var pattern = new RegExp("typedef(.*?)" + 
                ctags[i].pattern.substring(2, ctags[i].pattern.length - 2)
                .replace(/\*/g, "\\*")
                .replace(/\{/g, "\\{")
                .replace(/\}/g, "\\}")
                .replace(/\(/g, "\\(")
                .replace(/\)/g, "\\)")
                .replace(/\+/g, "\\+")
                .replace(/\//g, "\\/")
            , "gm");
            var final = pattern.exec(source.toString()
                .replace(/\s*\/\/.*$/mg, "")
                .replace(/\/\*(.|\r\n|\n)*?\*\//g, "")
                .replace(/(\n|\r|\t)/g, "")
            );
            if(final == null) continue;
            fs.writeSync(Symbols, final[0].replace(/;/g, ";\n"));
        }
    }
}
fs.writeSync(Symbols, "\n");

var getAddress = ((tab) => {
    for(var j = MapStart; j <= MapEnd; j++){
        var location = map[j].split(' ');
        if(location.length != 2) continue;
        if(location[1] == tab.name){
            return location[0];
        }
    }
    return null;
});

fs.writeSync(Symbols, "/* Functions */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern kind (typeref, macrodef)
    if(ctags[i].kind == "function"){
        //#define FunctionName ((ReturnType(*)(Arguments))(void*)address)
        var address = getAddress(ctags[i]);
        if(address == null || /weak/.test(ctags[i].pattern) || ctags[i].name == "main"){
            //console.warn("Warning: Cannot found symbol " + ctags[i].name + " in map file.");
            continue;
        }
        fs.writeSync(Symbols, "#define " + ctags[i].name + " ");
        fs.writeSync(Symbols, "((" + ctags[i].typeref.substring(9) + "(*)" + ((tab) => {
            var source = fs.readFileSync(tab.path);
            var pattern = new RegExp(tab.typeref.substring(9).replace(/\*/g, "\\*") + "(\\s*)" + 
                tab.name + "(\\s*)(.+?\\))", "gm");
            var final = pattern.exec(source.toString().replace(/(\n|\r|\t)/g, ""));
            assert(final.length > 2);
            return final[2];
        })(ctags[i]) + ")(void*)" + address + ")\n");
    }
}
fs.writeSync(Symbols, "\n");

fs.writeSync(Symbols, "/* Global Variables */\n");
for(var i = 0; i < ctags.length; i++){
    // key: _type name path pattern file kind (typeref, macrodef)
    if(ctags[i].kind == "variable"){
        //#define VariablePointer ((Type*)(void*)address)
        var address = getAddress(ctags[i]);
        if(address == null){
            //console.log("Warning: Cannot found symbol " + ctags[i].name + " in map file.");
            continue;
        }
        fs.writeSync(Symbols, "#define " + ctags[i].name + " ");
        fs.writeSync(Symbols, "((" + ctags[i].typeref.substring(9) + "*)(void*)" + address + ")\n");
    }
}