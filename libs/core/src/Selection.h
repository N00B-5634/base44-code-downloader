#ifndef YELLOWDOCS_CORE_SELECTION_H
#define YELLOWDOCS_CORE_SELECTION_H

#include <SupportDefs.h>

namespace YellowDocs {
namespace Core {

class Selection {
public:
	Selection();
	Selection(int32 start, int32 end);
	Selection(const Selection& other);
	Selection& operator=(const Selection& other);
	~Selection();

	// Position management
	void SetStart(int32 start);
	int32 Start() const;

	void SetEnd(int32 end);
	int32 End() const;

	void SetRange(int32 start, int32 end);
	
	// Length
	int32 Length() const;

	// Check if valid
	bool IsValid() const;

	// Check if empty
	bool IsEmpty() const;

	// Check if single position
	bool IsSinglePosition() const;

	// Check if position is within selection
	bool Contains(int32 position) const;

	// Check if selection overlaps with another
	bool Overlaps(const Selection& other) const;

	// Check if selection is before another
	bool IsBefore(const Selection& other) const;

	// Check if selection is after another
	bool IsAfter(const Selection& other) const;

	// Normalize selection (ensure start <= end)
	void Normalize();

	// Expand to include a position
	void ExpandTo(int32 position);

	// Expand to include another selection
	void ExpandTo(const Selection& other);

	// Intersection with another selection
	Selection Intersection(const Selection& other) const;

	// Union with another selection
	Selection Union(const Selection& other) const;

	// Comparison
	bool operator==(const Selection& other) const;
	bool operator!=(const Selection& other) const;

	// Reset
	void Reset();

private:
	int32 fStart;
	int32 fEnd;
};

} // namespace Core
} // namespace YellowDocs

#endif // YELLOWDOCS_CORE_SELECTION_H
