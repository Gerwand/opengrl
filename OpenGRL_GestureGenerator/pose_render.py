import random
import bpy
import bpy_extras
import mathutils
from math import radians, degrees

# Number of poses in the library - unfortunately, don't know the automatic way to get it
pose_num = 12
samples_pose = 5000

filepath = "//generated-train2"

bpy.data.scenes["Scene"].node_tree.nodes["File Output"].base_path = filepath

hand_armature = bpy.data.objects["HandArmature"]
hand_model = bpy.data.objects["HandModel"]
scene = bpy.context.scene
camera = bpy.data.objects["Camera"]
scene.objects.active = hand_armature
bpy.ops.object.mode_set(mode='POSE', toggle=False)

scene.render.resolution_x = 512
scene.render.resolution_y = 424
id = 0

# Joints with 6 DoF
bone_6_names = ["Wrist"]
# Joints with 2 DoF
bone_2_names = ["Thumb.base", "Index.base", "Middle.base", "Ring.base", "Pinky.base"]
# Joints with 1 DoF
bone_1_names = ["Thumb.top", "Thumb.middle", "Index.middle", "Index.top", "Middle.middle", "Middle.top", "Ring.middle", "Ring.top", "Pinky.middle", "Pinky.top"]

# Get the bones
bone_6 = []
bone_2 = []
bone_1 = []
for name in bone_6_names:
    bone_6.append(hand_armature.pose.bones[name])
for name in bone_2_names:
    bone_2.append(hand_armature.pose.bones[name])
for name in bone_1_names:
    bone_1.append(hand_armature.pose.bones[name])

noise_axis = 'ZXY'

# Modify the rotations and transformations
for i in range(1, pose_num+1):
    for k in range(0, samples_pose):
        bpy.ops.pose.select_all(action='SELECT')
        bpy.ops.poselib.apply_pose(pose_index=i)
        for bone in bone_2:
            bone.rotation_mode = 'XYZ'
            noise_angle = [random.gauss(0, 3), random.gauss(0, 3)]
            for n in range(0, 2):
                bone.rotation_euler.rotate_axis(noise_axis[n], radians(noise_angle[n]))
        for bone in bone_1:
            bone.rotation_mode = 'XYZ'
            noise_angle = random.gauss(0, 3)
            bone.rotation_euler.rotate_axis(noise_axis[0], radians(noise_angle))
        for bone in bone_6:
            bone.rotation_mode = 'XYZ'
            noise_angle = [random.gauss(0, 6), random.gauss(0, 6), random.gauss(0, 6)]
            #co_2d = bpy_extras.object_utils.world_to_camera_view(scene, camera, )
            #print("2D Coords:", co_2d)

            for n in range(0, 3):
                bone.rotation_euler.rotate_axis(noise_axis[n], radians(noise_angle[n]))
            
            # Make sure that object fits in the camera viewport
            bb_valid = False
            while not bb_valid:
                #z_noise = random.gauss(6, 3)
                #if z_noise < 1:
                #    z_noise = 1
                #if z_noise > 11:
                #    z_noise = 11
                noise_position = mathutils.Vector((random.gauss(0, 3),  0, random.gauss(-1, 3)))
                bone.location = hand_armature.matrix_world.inverted() * bone.bone.matrix_local.inverted() * noise_position
                # Check if bounding box is inside the camera
                #global_loc = bone.id_data.matrix_world * bone.matrix * bone.location
                #print("global")
                #print(bone.location)
                #print(global_loc)
                bb_valid = True
                for b in range(0, 8):
                    bb = hand_armature.bound_box[b]
                    # Tranform bb by the global loc of the bone
                    vec = mathutils.Vector((bb[0], bb[1], bb[2])) + noise_position
                    camera_coord = bpy_extras.object_utils.world_to_camera_view(scene, camera, vec)
                    #print("cc")
                    #print(vec)
                    #print(camera_coord)
                    for c in camera_coord[0:-1]:
                        bb_valid = bb_valid and c > 0.1 and c < 0.9
        bpy.context.scene.frame_current = id
        print("Generating and saving file " + '%d/%d' % (id+1, pose_num*samples_pose))
        bpy.ops.render.render()
        id += 1
