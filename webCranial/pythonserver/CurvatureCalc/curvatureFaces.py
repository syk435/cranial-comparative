import numpy
import json
import sys

def normalize_v3(arr):
    ''' Normalize a numpy array of 3 component vectors shape=(n,3) '''
    lens = numpy.sqrt( arr[:,0]**2 + arr[:,1]**2 + arr[:,2]**2 )
    #print lens
    i = 0
    for len in numpy.nditer(lens, op_flags=['readwrite']):
        if len == 0:
            ##print "index: %d" % (i)
            len +=1;
        i +=1
    arr[:,0] /= lens
    arr[:,1] /= lens
    arr[:,2] /= lens                
    return arr

def calcCurvature(p0, p1, n0, n1):
    curvature = (numpy.dot(numpy.subtract(n1,n0),numpy.subtract(p1,p0)))/(numpy.dot(numpy.subtract(p1,p0),numpy.subtract(p1,p0)))
    return curvature

if __name__ == "__main__":
    
    #read points and faces from obj into multi-arrays
    vertices = numpy.array([]).reshape(0,3)
    faces = numpy.array([]).reshape(0,3)
    #add empty at start to make 1-indexed, not 0-indexed
    vertices = numpy.vstack([vertices,[0,0,0]])
    
    with open(sys.argv[1], "r") as filestream:
        for line in filestream:
            if line.startswith("v "):
                line = line[2:]
                currentLine = line.split(" ")
                currentLine[2] = currentLine[2][:-1]
                currentVertex = numpy.array(map(float, currentLine))
                vertices = numpy.vstack([vertices,currentVertex])
            elif line.startswith("f "):
                line = line[2:]
                currentLine = line.split(" ")
                currentLine[2] = currentLine[2][:-1]
                currentFace = numpy.array(map(int, currentLine))
                faces = numpy.vstack([faces,currentFace])
    ##print "Number of Vertices: %d" % (len(vertices))
    ##print "Number of Faces: %d\n" % (len(faces))
    faces = faces.astype(int)

    #Create a zeroed array with the same type and shape as our vertices i.e., per vertex normal
    norm = numpy.zeros( vertices.shape, dtype=vertices.dtype )
    #Create an indexed view into the vertex array using the array of three indices for triangles
    tris = vertices[faces]
    #Calculate the normal for all the triangles, by taking the cross product of the vectors v1-v0, and v2-v0 in each triangle             
    n = numpy.cross( tris[::,1 ] - tris[::,0]  , tris[::,2 ] - tris[::,0] )
    #print n
    # n is now an array of normals per triangle. The length of each normal is dependent the vertices, 
    # we need to normalize these, so that our next step weights each normal equally.
    normalize_v3(n)
    # now we have a normalized array of normals, one per triangle, i.e., per triangle normals.
    # But instead of one per triangle (i.e., flat shading), we add to each vertex in that triangle, 
    # the triangles' normal. Multiple triangles would then contribute to every vertex, so we need to normalize again afterwards.
    # The cool part, we can actually add the normals through an indexed view of our (zeroed) per vertex normal array
    norm[ faces[:,0] ] += n
    norm[ faces[:,1] ] += n
    norm[ faces[:,2] ] += n
    norm = numpy.delete(norm, (0), axis=0)
    #print len(norm)
    #print norm
    normalize_v3(norm)
    norm = numpy.insert(norm, (0), [0,0,0], axis = 0)
    ##print len(norm)
    
    curvaturesDict = {}
    facesDict = {}
    i = 1
    for face in faces:
        facesDict[i] = face.tolist()
        i+=1
    ##print facesDict
    with open(sys.argv[1]+"faces.json", "w") as f:
        json.dump(facesDict,f)