#pragma once
#include <windows.h>

class CVFTManager {
private:
	void** original_vft = nullptr;
	void** current_vft = nullptr;
	unsigned int FnCount;
	
	inline const void SetHook(void* hook, const unsigned int i) {
		if (i > this->FnCount) return;
		unsigned long protect;
		::VirtualProtect(&this->current_vft[i], 4, PAGE_READWRITE, &protect);
		this->current_vft[i] = hook;
		::VirtualProtect(&this->current_vft[i], 4, protect, &protect);
	}

public:
	CVFTManager(void** VFT, const unsigned int count) {
		this->FnCount = count;
		this->original_vft = reinterpret_cast<void**>(::VirtualAlloc(NULL, this->FnCount * sizeof(void*), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
		this->current_vft = VFT;
		::memcpy(this->original_vft, this->current_vft, this->FnCount * sizeof(void*));
	};

	template <typename Fn> inline const Fn HookFn(void* hook, const unsigned int i) {
		this->SetHook(hook, i);
		return reinterpret_cast<Fn>(this->original_vft[i]);
	}
	inline const void UnhookFn(const unsigned int i) {
		this->SetHook(this->original_vft[i], i);
	}

	template <typename Fn> inline const Fn GetOriginalFn(const unsigned int i) {
		return reinterpret_cast<Fn>(this->original_vft[i]);
	}
	template <typename Fn> inline const Fn GetCurrentFn(const unsigned int i) {
		return reinterpret_cast<Fn>(this->current_vft[i]);
	}
	inline const unsigned int GetFnCount(void) {
		return this->FnCount;
	}
};
