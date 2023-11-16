import bpy, json

if not bpy.data.is_saved:
    raise Error("File not saved!")

originCollection = bpy.data.collections.get("ORIGIN")
createdCollection = bpy.data.collections.get("CREATED")

if originCollection and createdCollection:
    print(f"Found {len(originCollection.objects)} objects in origin collection")
    print(f"Found {len(createdCollection.objects)} objects in created collection")
    
    dictObj = dict()

    for originalObj in originCollection.objects:
        originalName = originalObj.name
        
        for createdObj in createdCollection.objects:
            if createdObj.name.startswith(originalName):
                if originalName in dictObj:
                    objList = dictObj[originalName]
                else:
                    objList = []
                    
                objList.append({
                    "position": {
                        "x": createdObj.location.x,
                        "y": createdObj.location.y,
                        "z": createdObj.location.z
                    },
                    "rotation": {
                        "x": createdObj.rotation_euler.x,
                        "y": createdObj.rotation_euler.y,
                        "z": createdObj.rotation_euler.z
                    },
                    "scale": {
                        "x": createdObj.scale.x,
                        "y": createdObj.scale.y,
                        "z": createdObj.scale.z
                    }
                })
                dictObj[originalName] = objList
                    
    print(dictObj)
    
    data = json.dumps(dictObj, indent=1, ensure_ascii=True)
    
    file_name = bpy.path.abspath(r"//map_data.json")

    with open(file_name, 'w') as outfile:
        outfile.write(data + '\n')

else:
    print("Error, one of the collection is missing")
