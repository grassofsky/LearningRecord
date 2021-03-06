import glm

class ProjectionType:
    PERSPECTIVE = 0
    ORTHOGONAL = 1

class Camera:
    '''
        Class camera 
        Used for control the perspective for rendering
    '''

    def __init__(self):
        self._eye = glm.vec3(0,0,100)
        self._look_at = glm.vec3(0,0,0)
        self._updirection = glm.vec3(0,1,1)
        self._fov_y = 1.0
        self._aspect = 1.0
        self._near_dist = 0.2
        self._far_dist = 10
        self._ortho_height = 1.0
        self._bview_matrix_dirty = True
        self._bperspective_matrix_dirty = True
        self._proj_type = ProjectionType.PERSPECTIVE
        self._view_matrix = glm.mat4()
        self._proj_matrix = glm.mat4()


    @property
    def eye(self):
        '''
            np array shape is (3,1):
            Get/Set the position of eye
        '''
        return self._eye

    @eye.setter
    def eye(self, value):
        self._eye = value

    
    
        

