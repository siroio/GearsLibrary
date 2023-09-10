#pragma once
#define UCHAR unsigned char
#define UINT unsigned int

namespace Glib::Internal::Graphics::ID
{
    /* ���_���\�[�XID */

    constexpr UCHAR CAMERA_VERTEX = 0;
    constexpr UCHAR IMAGE_VERTEX = CAMERA_VERTEX + 1;

    /* �p�C�v���C��ID */

    constexpr UCHAR CAMERA_PIPELINESTATE = 0;                                           /* �J���� */
    constexpr UCHAR SPRITE_PIPELINESTATE = CAMERA_PIPELINESTATE + 1;                    /* �X�v���C�g */
    constexpr UCHAR IMAGE_PIPELINESTATE = SPRITE_PIPELINESTATE + 1;                     /* UI */
    constexpr UCHAR LINE_PIPELINESTATE = IMAGE_PIPELINESTATE + 1;                       /* ���C�� */

    constexpr UCHAR MESH_PIPELINESTATE = LINE_PIPELINESTATE + 1;                        /* ���b�V�� */
    constexpr UCHAR MESH_SHADOW_PIPELINESTATE = MESH_PIPELINESTATE + 1;                 /* ���b�V�� */

    constexpr UCHAR SKINNED_MESH_PIPELINESTATE = MESH_SHADOW_PIPELINESTATE + 1;         /* �X�L�j���O���b�V��  */
    constexpr UCHAR SKINNED_MESH_SHADOW_PIPELINESTATE = SKINNED_MESH_PIPELINESTATE + 1; /* �X�L�j���O���b�V��  */

    /* �V�F�[�_�[ID */

    constexpr UCHAR SPRITE_SHADER = 0;
    constexpr UCHAR CAMERA_SHADER = SPRITE_SHADER + 1;
    constexpr UCHAR IMAGE_SHADER = CAMERA_SHADER + 1;
    constexpr UCHAR LINE_SHADER = IMAGE_SHADER + 1;
    constexpr UCHAR MESH_SHADER = LINE_SHADER + 1;
    constexpr UCHAR MESH_SHADOW_SHADER = MESH_SHADER + 1;

    /*  �V�F�[�_�[�p�����[�^�[ID  */

    constexpr UINT MESH_ALBEDO = 0;
    constexpr UINT MESH_NORMAL_MAP = 1;
    constexpr UINT MESH_MATERIAL_BUFFER = 4;

    constexpr UINT MESH_CAMERA_CONSTANT = 2;
    constexpr UINT MESH_WORLD_MATRIX = 3;
    constexpr UINT MESH_DIRECTIONAL_LIGHT = 5;
    constexpr UINT MESH_SHADOW_MAP = 6;

    constexpr UINT SKINNED_MESH_CAMERA_CONSTANT = 2;
    constexpr UINT SKINNED_MESH_WORLD_MATRIX = 3;
    constexpr UINT SKINNED_MESH_SKINNED_MATRIX = 5;
    constexpr UINT SKINNED_MESH_DIRECTIONAL_LIGHT = 6;
    constexpr UINT SKINNED_MESH_SHADOW_MAP = 7;

    /* �e�N�X�`��ID */

    constexpr UCHAR WHITE_TEXTURE = 0;
    constexpr UCHAR NORMAL_MAP_TEXTURE = WHITE_TEXTURE + 1;
}

#undef UCHAR
#undef UINT
