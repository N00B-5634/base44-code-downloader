#include "Selection.h"

#include <algorithm>

namespace YellowDocs {
namespace Core {

Selection::Selection()
	: fStart(-1),
	  fEnd(-1)
{
}

Selection::Selection(int32 start, int32 end)
	: fStart(start),
	  fEnd(end)
{
	Normalize();
}

Selection::Selection(const Selection& other)
	: fStart(other.fStart),
	  fEnd(other.fEnd)
{
}

Selection& Selection::operator=(const Selection& other) {
	if (this != &other) {
		fStart = other.fStart;
		fEnd = other.fEnd;
	}
	return *this;
}

Selection::~Selection() {
}

// Position management

void Selection::SetStart(int32 start) {
	fStart = start;
	Normalize();
}

int32 Selection::Start() const {
	return fStart;
}

void Selection::SetEnd(int32 end) {
	fEnd = end;
	Normalize();
}

int32 Selection::End() const {
	return fEnd;
}

void Selection::SetRange(int32 start, int32 end) {
	fStart = start;
	fEnd = end;
	Normalize();
}

// Length

int32 Selection::Length() const {
	if (!IsValid()) {
		return 0;
	}
	return fEnd - fStart + 1;
}

// Check if valid

bool Selection::IsValid() const {
	return fStart >= 0 && fEnd >= 0 && fStart <= fEnd;
}

// Check if empty

bool Selection::IsEmpty() const {
	return IsValid() && fStart == fEnd;
}

// Check if single position

bool Selection::IsSinglePosition() const {
	return IsValid() && fStart == fEnd;
}

// Check if position is within selection

bool Selection::Contains(int32 position) const {
	return IsValid() && position >= fStart && position <= fEnd;
}

// Check if selection overlaps with another

bool Selection::Overlaps(const Selection& other) const {
	if (!IsValid() || !other.IsValid()) {
		return false;
	}
	return !(fEnd < other.fStart || other.fEnd < fStart);
}

// Check if selection is before another

bool Selection::IsBefore(const Selection& other) const {
	if (!IsValid() || !other.IsValid()) {
		return false;
	}
	return fEnd < other.fStart;
}

// Check if selection is after another

bool Selection::IsAfter(const Selection& other) const {
	if (!IsValid() || !other.IsValid()) {
		return false;
	}
	return fStart > other.fEnd;
}

// Normalize selection (ensure start <= end)

void Selection::Normalize() {
	if (fStart > fEnd) {
		std::swap(fStart, fEnd);
	}
}

// Expand to include a position

void Selection::ExpandTo(int32 position) {
	if (!IsValid()) {
		fStart = position;
		fEnd = position;
		return;
	}
	
	if (position < fStart) {
		fStart = position;
	} else if (position > fEnd) {
		fEnd = position;
	}
}

// Expand to include another selection

void Selection::ExpandTo(const Selection& other) {
	if (!other.IsValid()) {
		return;
	}
	
	if (!IsValid()) {
		fStart = other.fStart;
		fEnd = other.fEnd;
		return;
	}
	
	if (other.fStart < fStart) {
		fStart = other.fStart;
	}
	if (other.fEnd > fEnd) {
		fEnd = other.fEnd;
	}
}

// Intersection with another selection

Selection Selection::Intersection(const Selection& other) const {
	if (!IsValid() || !other.IsValid() || !Overlaps(other)) {
		return Selection();
	}
	
	return Selection(std::max(fStart, other.fStart), std::min(fEnd, other.fEnd));
}

// Union with another selection

Selection Selection::Union(const Selection& other) const {
	if (!IsValid()) {
		return other;
	}
	if (!other.IsValid()) {
		return *this;
	}
	
	return Selection(std::min(fStart, other.fStart), std::max(fEnd, other.fEnd));
}

// Comparison

bool Selection::operator==(const Selection& other) const {
	return fStart == other.fStart && fEnd == other.fEnd;
}

bool Selection::operator!=(const Selection& other) const {
	return !(*this == other);
}

// Reset

void Selection::Reset() {
	fStart = -1;
	fEnd = -1;
}

} // namespace Core
} // namespace YellowDocs
