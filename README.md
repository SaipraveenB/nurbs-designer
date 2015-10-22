# nurbs-designer
NURBS( Non-Uniform Rational B-Splines ) curve rendering engine.

NURBSEditor.java is the Java GUI that looks like this:
![gui.png]({{site.baseurl}}/gui.png)

NURBSSurface.java holds the construction algorithm for the 2D basis functions.( They're like 3D sinusoids( with two parameters ) but unlike sinusoids, the definition for the NURBS basis is such that they're fundamentally _local_ in nature which is a much nicer function for designers to work with.

For example: an order 3 NURBS surface plotted with 1600 samples ( 40x40 )
![o_3_s_40.png]({{site.baseurl}}/o_3_s_40.png)

Note that the distortion on the corners have whatsoever no effect beyond the neighbouring control points.

For better clarity, see the order 2 surface which is basically linear in nature.


** Description and Synopsis coming soon **


Check out some of my other projects here:
OpenCL path trapcer: https://github.com/SaipraveenB/opencl-path-tracer.git
OpenGL HDR example:
https://github.com/Saipraveenb/opengl-hdr.git
