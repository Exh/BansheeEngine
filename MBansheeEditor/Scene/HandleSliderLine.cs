﻿using System;
using System.Runtime.CompilerServices;
using BansheeEngine;

namespace BansheeEditor
{
    public sealed class HandleSliderLine : HandleSlider
    {
        public HandleSliderLine(Handle parentHandle, Vector3 direction, float length,  bool fixedScale = true, float snapValue = 0.0f)
            :base(parentHandle)
        {
            Internal_CreateInstance(this, direction, length, fixedScale, snapValue);
        }

        public float Delta
        {
            get
            {
                float value;
                Internal_GetDelta(mCachedPtr, out value);
                return value;
            }
        }

        public Vector3 NewPosition
        {
            get
            {
                Vector3 value;
                Internal_GetNewPosition(mCachedPtr, out value);
                return value;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_CreateInstance(HandleSliderLine instance, Vector3 direction, float length, bool fixedScale, float snapValue);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetDelta(IntPtr nativeInstance, out float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetNewPosition(IntPtr nativeInstance, out Vector3 value);
    }
}