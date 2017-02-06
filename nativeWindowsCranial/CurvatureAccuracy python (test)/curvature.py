import sys
sys.path.append("C:\\Program Files (x86)\\VTK 6.3.0\\bin\\Lib")
sys.path.append("C:\\Program Files (x86)\\VTK 6.3.0\\bin\\Lib\\site-packages")
import vtk
import numpy

#setup
class CurvaturesDemo():
 
    def CurvaturesDemo(self):
 
        # We are going to handle two different sources.
        # The first source is a superquadric source.
        torus = vtk.vtkSuperquadricSource();
        torus.SetCenter(0.0, 0.0, 0.0)
        torus.SetScale(1.0, 1.0, 1.0)
        torus.SetPhiResolution (64)
        torus.SetThetaResolution(64)
        torus.SetThetaRoundness (1)
        torus.SetThickness (0.5)
        torus.SetSize(0.5)
        torus.SetToroidal(1) 
 
        # Rotate the torus towards the observer (around the x-axis)
        torusT = vtk.vtkTransform()
        torusT.RotateX(55)
 
        torusTF = vtk.vtkTransformFilter()
        torusTF.SetInputConnection(torus.GetOutputPort())
        torusTF.SetTransform(torusT)
 
        # The quadric is made of strips, so pass it through a triangle filter as
        # the curvature filter only operates on polys
        tri = vtk.vtkTriangleFilter()
        tri.SetInputConnection(torusTF.GetOutputPort())
 
        # The quadric has nasty discontinuities from the way the edges are generated
        # so let's pass it though a CleanPolyDataFilter and merge any points which
        # are coincident, or very close
 
        cleaner = vtk.vtkCleanPolyData()
        cleaner.SetInputConnection(tri.GetOutputPort())
        cleaner.SetTolerance(0.005)
 
        # The next source will be a parametric function
        rh = vtk.vtkParametricRandomHills()
        rhFnSrc = vtk.vtkParametricFunctionSource()
        rhFnSrc.SetParametricFunction(rh)
 
        # Now we have the sources, lets put them into a list.
        sources = list()
        sources.append(cleaner)
        sources.append(cleaner)
        sources.append(rhFnSrc)
        sources.append(rhFnSrc)
 
        # Colour transfer function.
        ctf = vtk.vtkColorTransferFunction()
        ctf.SetColorSpaceToDiverging()
        ctf.AddRGBPoint(0.0, 0.230, 0.299, 0.754)
        ctf.AddRGBPoint(1.0, 0.706, 0.016, 0.150)
        cc = list()
        for i in range(256):
            cc.append(ctf.GetColor(float(i) / 255.0)) 
 
        # Lookup table.
        lut = list()
        for idx in range(len(sources)):
            lut.append(vtk.vtkLookupTable())
            lut[idx].SetNumberOfColors(256)
            for i, item in enumerate(cc):
                lut[idx].SetTableValue(i, item[0], item[1], item[2], 1.0)
            if idx == 0:
                lut[idx].SetRange(-10, 10)
            if idx == 1:
                lut[idx].SetRange(0, 4)
            if idx == 2:
                lut[idx].SetRange(-1, 1)
            if idx == 3:
                lut[idx].SetRange(-1, 1)
            lut[idx].Build()
 
        curvatures = list()        
        for idx in range(len(sources)):
            curvatures.append(vtk.vtkCurvatures())
            if idx % 2 == 0:
                curvatures[idx].SetCurvatureTypeToGaussian()
            else:
                curvatures[idx].SetCurvatureTypeToMean()
 
        renderers = list()
        mappers = list()
        actors = list()
        textmappers = list()
        textactors = list()
 
        # Create a common text property.
        textProperty = vtk.vtkTextProperty()
        textProperty.SetFontSize(10)
        textProperty.SetJustificationToCentered()
 
        names = ['Torus - Gaussian Curvature', 'Torus - Mean Curvature', 'Gaussian Curvature', 'Mean Curvature']
 
        # Link the pipeline together. 
        for idx, item in enumerate(sources):
            sources[idx].Update()
 
            curvatures[idx].SetInputConnection(sources[idx].GetOutputPort())
 
            mappers.append(vtk.vtkPolyDataMapper())
            mappers[idx].SetInputConnection(curvatures[idx].GetOutputPort())
            mappers[idx].SetLookupTable(lut[idx])
            mappers[idx].SetUseLookupTableScalarRange(1)
 
            actors.append(vtk.vtkActor())
            actors[idx].SetMapper(mappers[idx])
 
            textmappers.append(vtk.vtkTextMapper())
            textmappers[idx].SetInput(names[idx])
            textmappers[idx].SetTextProperty(textProperty)
 
            textactors.append(vtk.vtkActor2D())
            textactors[idx].SetMapper(textmappers[idx])
            textactors[idx].SetPosition(150, 16)
 
            renderers.append(vtk.vtkRenderer())
 
        gridDimensions = 2
 
        for idx in range(len(sources)):
            if idx < gridDimensions * gridDimensions:
                renderers.append(vtk.vtkRenderer)
 
        rendererSize = 300
 
        # Create the RenderWindow
        #
        renderWindow = vtk.vtkRenderWindow()
        renderWindow.SetSize(rendererSize * gridDimensions, rendererSize * gridDimensions)
 
        # Add and position the renders to the render window.
        viewport = list()
        for row in range(gridDimensions):
            for col in range(gridDimensions):
                idx = row * gridDimensions + col
 
                viewport[:] = []
                viewport.append(float(col) * rendererSize / (gridDimensions * rendererSize))
                viewport.append(float(gridDimensions - (row+1)) * rendererSize / (gridDimensions * rendererSize))
                viewport.append(float(col+1)*rendererSize / (gridDimensions * rendererSize))
                viewport.append(float(gridDimensions - row) * rendererSize / (gridDimensions * rendererSize))
 
                if idx > (len(sources) - 1):
                    continue
 
                renderers[idx].SetViewport(viewport)
                renderWindow.AddRenderer(renderers[idx])
 
                renderers[idx].AddActor(actors[idx])
                renderers[idx].AddActor(textactors[idx])
                renderers[idx].SetBackground(0.4,0.3,0.2)
 
        interactor = vtk.vtkRenderWindowInteractor()
        interactor.SetRenderWindow(renderWindow)
 
        renderWindow.Render()
 
        interactor.Start()

def normalize_v3(arr):
    ''' Normalize a numpy array of 3 component vectors shape=(n,3) '''
    lens = numpy.sqrt( arr[:,0]**2 + arr[:,1]**2 + arr[:,2]**2 )
    #print lens
    i = 0
    for len in numpy.nditer(lens, op_flags=['readwrite']):
        if len == 0:
            print "index: %d" % (i)
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
    ##po = CurvaturesDemo()
    ##po.CurvaturesDemo()

    #read points and faces from obj into multi-arrays
    vertices = numpy.array([]).reshape(0,3)
    faces = numpy.array([]).reshape(0,3)
    #add empty at start to make 1-indexed, not 0-indexed
    vertices = numpy.vstack([vertices,[0,0,0]])
    
    with open("Patient1_decimated96percent_7978v.obj", "r") as filestream:
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
    print "Number of Vertices: %d" % (len(vertices))
    print "Number of Faces: %d\n" % (len(faces))
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
    print len(norm)
    
    curvaturesDict = {}
    for face in faces:
        if face[0] in curvaturesDict:
            curvaturesDict[face[0]].append(calcCurvature(vertices[face[0]],vertices[face[1]],norm[face[0]],norm[face[1]]))
            curvaturesDict[face[0]].append(calcCurvature(vertices[face[0]],vertices[face[2]],norm[face[0]],norm[face[2]]))
        else:
            curvaturesDict[face[0]] = [calcCurvature(vertices[face[0]],vertices[face[1]],norm[face[0]],norm[face[1]])]
            curvaturesDict[face[0]].append(calcCurvature(vertices[face[0]],vertices[face[2]],norm[face[0]],norm[face[2]]))

        if face[1] in curvaturesDict:
            curvaturesDict[face[1]].append(calcCurvature(vertices[face[1]],vertices[face[0]],norm[face[1]],norm[face[0]]))
            curvaturesDict[face[1]].append(calcCurvature(vertices[face[1]],vertices[face[2]],norm[face[1]],norm[face[2]]))
        else:
            curvaturesDict[face[1]] = [calcCurvature(vertices[face[1]],vertices[face[0]],norm[face[1]],norm[face[0]])]
            curvaturesDict[face[1]].append(calcCurvature(vertices[face[1]],vertices[face[2]],norm[face[1]],norm[face[2]]))

        if face[2] in curvaturesDict:
            curvaturesDict[face[2]].append(calcCurvature(vertices[face[2]],vertices[face[1]],norm[face[2]],norm[face[1]]))
            curvaturesDict[face[2]].append(calcCurvature(vertices[face[2]],vertices[face[0]],norm[face[2]],norm[face[0]]))
        else:
            curvaturesDict[face[2]] = [calcCurvature(vertices[face[2]],vertices[face[1]],norm[face[2]],norm[face[1]])]
            curvaturesDict[face[2]].append(calcCurvature(vertices[face[2]],vertices[face[0]],norm[face[2]],norm[face[0]]))

    ##NEEED TO REMOVE ALL REPEAT VALUES IN EACH LIST!!!
    #print len(curvaturesDict)
    #print curvaturesDict[1]
    with open("curvature.csv", "w") as stream:
        for i in xrange(1,7979):
            maxp = curvaturesDict[i][0]
            minp = curvaturesDict[i][0]
            #weightedAvg = 0
            for curve in curvaturesDict[i]:
                if curve > maxp:
                    maxp = curve
                if curve < minp:
                    minp = curve
                #weightedAvg += curve
            #print "Max: %f  Min: %f  Gaussian: %f  Mean: %f  GeomMean: %f" % (maxp,minp,(maxp*minp),((maxp+minp)/2),(weightedAvg/len(curvaturesDict[i])))
            #print "%d: Gaussian: %f  Mean: %f" % (i,(maxp*minp),((maxp+minp)/2))
            stream.write("%f,%f\n"%((maxp*minp),((maxp+minp)/2)))
